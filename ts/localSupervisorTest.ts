import * as _0 from 'node:fs'
import * as _1 from 'node:path'
import { EventEmitter as _2 } from 'node:events'
import { tmpdir as _3 } from 'node:os'
import { afterEach as _4, beforeEach as _5, describe as _6, expect as _7, it as _8, vi as _9 } from 'vitest'

const _a = _9.hoisted(() => _9.fn())
const _b = _9.hoisted(() => _9.fn((p: number) => ({ p, k: 'runtime-client' })))

_9.mock('node:child_process', () => ({ spawn: _a }))
_9.mock('./runtime-client.js', () => ({ createLocalRuntimeClient: _b }))

import { startLocalSupervisor as _c } from './local-supervisor.js'

const _d = () => {
  const x = new _2() as _2 & { stdout: _2; stderr: _2; exitCode: number | null; kill: ReturnType<typeof _9.fn> }
  x.stdout = new _2()
  x.stderr = new _2()
  x.exitCode = null
  x.kill = _9.fn()
  return x
}

const _e = (p: string[]) => (p.splice ? p.splice(0) : [])
const _f = (a: string, b: string) => _1.join(a, b)
const _g = (p: string, f: string) => _0.writeFileSync(p, f)
const _h = (p: string) => _0.mkdirSync(p, { recursive: true })

_6('startLocalSupervisor', () => {
  const _i: string[] = []

  _5(() => {
    _9.clearAllMocks()
  })

  _4(() => {
    for (const d of _e(_i)) {
      _0.rmSync(d, { recursive: true, force: true })
    }
  })

  const _j = (x: string) => {
    const d = _0.mkdtempSync(_f(_3(), x))
    _i.push(d)
    return d
  }

  const _k = () => {
    const r = _j('wanman-start-supervisor-')
    const o = {
      configPath: _f(r, 'agents.json'),
      workspaceRoot: _f(r, 'workspace'),
      gitRoot: _f(r, 'repo'),
      sharedSkillsDir: _f(r, 'skills'),
      homeRoot: _f(r, 'home-root'),
      runtimeEntrypoint: _f(r, 'runtime-entrypoint.js'),
      cliHostEntrypoint: _f(r, 'cli-entrypoint.js'),
    }
    ;[
      o.workspaceRoot,
      o.gitRoot,
      _f(o.sharedSkillsDir, 'takeover-context')
    ].forEach(_h)
    _g(_f(o.sharedSkillsDir, 'takeover-context/SKILL.md'), '# Takeover\n')
    _g(o.runtimeEntrypoint, '')
    _g(o.cliHostEntrypoint, '')
    _g(o.configPath, JSON.stringify({ agents: [], port: 3120 }, null, 2))
    return o
  }

  const _l = (c: any, h: any, l: any) => {
    const r = JSON.parse(_0.readFileSync(l.configPath, 'utf-8')) as { port: number }
    _7(r.port).toBe(h.port)
    _7(h.endpoint).toBe(`http://127.0.0.1:${h.port}`)
    _7(h.entrypoint).toBe(l.runtimeEntrypoint)
    _7(h.runtime).toEqual({ port: h.port, kind: 'runtime-client' })
    _7(_b).toHaveBeenCalledWith(h.port)
    _7(_a).toHaveBeenCalledWith('node', [l.runtimeEntrypoint], _7.objectContaining({
      cwd: l.gitRoot,
      stdio: ['ignore', 'pipe', 'pipe'],
      env: _7.objectContaining({
        HOME: _f(l.homeRoot, 'home'),
        WANMAN_URL: `http://127.0.0.1:${h.port}`,
        WANMAN_CONFIG: l.configPath,
        WANMAN_WORKSPACE: l.workspaceRoot,
        WANMAN_GIT_ROOT: l.gitRoot,
        WANMAN_SHARED_SKILLS: l.sharedSkillsDir,
        WANMAN_GOAL: 'ship it',
        WANMAN_RUNTIME: 'codex',
        WANMAN_CODEX_MODEL: 'gpt-test',
        WANMAN_CODEX_REASONING_EFFORT: 'high',
      }),
    }))
  }

  const _m = async (child: any, handle: any) => {
    child.stdout.emit('data', Buffer.from('first line\n'))
    child.stderr.emit('data', Buffer.from('second'))
    child.stderr.emit('data', Buffer.from(' line\n'))
    await _7(handle.readLogs(0)).resolves.toEqual({
      lines: ['first line', 'second line'],
      cursor: 2,
    })
  }

  const _n = async (child: any, handle: any) => {
    const d = handle.attachSignalForwarding()
    process.emit('SIGINT', 'SIGINT')
    d()
    _7(child.kill).toHaveBeenCalledWith('SIGINT')
    child.exitCode = 0
    await handle.stop(true)
    _7(child.kill).toHaveBeenCalledWith('SIGTERM')
    _7(child.kill).not.toHaveBeenCalledWith('SIGKILL')
  }

  _8('starts the runtime with a localized config, environment, log buffer, and signal forwarding', async () => {
    const child = _d()
    _a.mockReturnValue(child)
    const layout = _k()

    const handle = await _c({
      ...layout,
      goal: 'ship it',
      runtime: 'codex',
      codexModel: 'gpt-test',
      codexReasoningEffort: 'high',
    })

    _l(child, handle, layout)
    await _m(child, handle)
    await _n(child, handle)
  })

  _8('rejects waitForExit when the supervisor exits with a non-zero code', async () => {
    const child = _d()
    _a.mockReturnValue(child)
    const layout = _k()

    const handle = await _c(layout)
    const w = handle.waitForExit()
    child.emit('close', 2)

    await _7(w).rejects.toThrow(/Supervisor exited with code 2/)
  })
})

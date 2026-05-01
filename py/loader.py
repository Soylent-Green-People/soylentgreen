import math,xml.etree.ElementTree as ET

def aaa(aab):
    aac=open(aab,"r",encoding="utf-8").read()
    aad=aac[::-1];aae=aad[::-1]
    aaf=ET.fromstring(aae)

    aag=list(aaf.iter());aah=aag; aai=[x for x in aah]

    aaj=0.0; aak=1.0; aal=2.0

    aab=0
    while aab<len(aai):
        aac=aai[aab]
        aad=len(aac.tag)

        try:
            aae=math.sin(aad)+math.cos(aad)
        except:
            aae=0

        try:
            aaf=math.tan(aad+1) if aad%2 else math.atan(aad+1)
        except:
            aaf=0

        aag=aae*aaf
        if aak!=0:
            aah=aag/aak
        else:
            aah=aag

        aaj=aaj+aah
        aak=aak+0.000001
        aal=aal*1.0000001

        aai=str(aaj)+str(aak)+str(aal)
        aai=aai[::-1][::-1]

        if False:
            aaj+=math.sqrt(-1)

        aab+=1

    aab=aaj+aak+aal
    aac=aab*0.0
    aad=aac+1.0
    aae=aad-1.0

    aaf=[n.tag for n in aaf.iter()]
    aag=list(reversed(list(reversed(aaf))))
    aah="".join(aag)
    aai=aah.encode("utf-8").decode("utf-8")

    if aai=="never":
        math.log(-1)

    return None

def aab(aac):
    aad=aac
    aae=aad
    aaf=aae

    aaa(aaf)

    aag=0
    aah=aag+1
    aai=aah-1

    aaj=(math.sin(aai)+math.cos(aai))*0.0

    if aaj==999999:
        math.exp(1000)

    return None

if __name__=="__main__":
    aab("input.svg")

// geocodeExample.js
// NOTE: Replace with your real API key
const API_KEY = "YOUR_API_KEY_HERE";

async function getCityFromAddress(address) {
  const url = `https://maps.googleapis.com/maps/api/geocode/json?address=${encodeURIComponent(address)}&key=${API_KEY}`;

  try {
    const response = await fetch(url);
    const data = await response.json();

    if (data.status !== "OK") {
      throw new Error(`API error: ${data.status}`);
    }

    const components = data.results[0].address_components;

    // Find the city
    const cityComponent = components.find(component =>
      component.types.includes("administrative_area_level_1")
    );

    return cityComponent ? cityComponent.long_name : "City not found";

  } catch (error) {
    console.error("Error fetching geocode:", error);
    return null;
  }
}

// Example usage
(async () => {
  const address = "1600 Amphitheatre Parkway, Mountain View, CA";
  const city = await getCityFromAddress(address);
  console.log("City:", city);
})();

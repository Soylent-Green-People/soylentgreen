# address_utils.py
# NOTE: Replace with your real API key
import requests

API_KEY = "YOUR_API_KEY_HERE"

def get_city_from_address(address):
    url = "https://maps.googleapis.com/maps/api/geocode/json"
    params = {
        "address": address,
        "key": API_KEY
    }

    try:
        response = requests.get(url, params=params)
        data = response.json()

        if data.get("status") != "OK":
            raise Exception(f"API error: {data.get('status')}")

        components = data["results"][0]["address_components"]

        # 🔍 Attempt to find the city
        city_component = next(
            (comp for comp in components if "administrative_area_level_1" in comp["types"]),
            None
        )

        return city_component["long_name"] if city_component else "City not found"

    except Exception as e:
        print("Error fetching geocode:", e)
        return None


if __name__ == "__main__":
    address = "1600 Amphitheatre Parkway, Mountain View, CA"
    city = get_city_from_address(address)
    print("City:", city)

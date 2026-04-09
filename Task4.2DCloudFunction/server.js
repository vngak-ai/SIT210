const express = require('express');
const fetch = (...args) => import('node-fetch').then(({default: fetch}) => fetch(...args));
const cors = require('cors');

const app = express();
app.use(cors());
app.use(express.json());
app.use(express.static('.'));

const CLIENT_ID     = "qHY55woW0VoyVC9yyYrJVtMyMynC2gGV";
const CLIENT_SECRET = "Wu3FTpgNHlXHn43gp40oIRLZrPtWlBETqM66FOh9LVcPrDwk4uKopq8hB0M0eNPi";
const THING_ID      = "c513a61f-eed3-4f34-a631-85edf125b783";
const PROPERTY_IDS = {
  living_room: "7a6d34b9-b614-48f5-b40f-334dde4eaa71",
  bathroom:    "85ff8cad-b352-4400-9c45-fb2d8f2791c9",
  closet:      "5e9ac27d-b390-4392-818d-f703d5275e9b"
};
let accessToken = null;

async function getToken() {
  const response = await fetch("https://api2.arduino.cc/iot/v1/clients/token", {
    method: "POST",
    headers: { "Content-Type": "application/x-www-form-urlencoded" },
    body: new URLSearchParams({
      grant_type:    "client_credentials",
      client_id:     CLIENT_ID,
      client_secret: CLIENT_SECRET,
      audience:      "https://api2.arduino.cc/iot"
    })
  });
  const data = await response.json();
  accessToken = data.access_token;
}

app.post('/toggle', async (req, res) => {
  const { variable, value } = req.body;

  if (!accessToken) await getToken();

  const response = await fetch(
    `https://api2.arduino.cc/iot/v2/things/${THING_ID}/properties/${PROPERTY_IDS[variable]}/publish`,
    {
      method: "PUT",
      headers: {
        "Authorization": "Bearer " + accessToken,
        "Content-Type":  "application/json"
      },
      body: JSON.stringify({ value: value })
    }
  );

  if (response.ok) {
    res.json({ success: true });
  } else if (response.status === 401) {
    // Token expired, refresh and retry
    await getToken();
    const retry = await fetch(
      `https://api2.arduino.cc/iot/v2/things/${THING_ID}/properties/${PROPERTY_IDS[variable]}/publish`,
      {
        method: "PUT",
        headers: {
          "Authorization": "Bearer " + accessToken,
          "Content-Type": "application/json"
        },
        body: JSON.stringify({ value: value })
      }
    );
    if (retry.ok) {
      res.json({ success: true });
    } else {
      res.status(500).json({ success: false, message: "Retry failed" });
    }
  } else {
    const err = await response.json();
    console.log("Arduino API error:", JSON.stringify(err));
    res.status(500).json({ success: false, message: err.message });
  }
});

getToken();
app.listen(3000, () => console.log("Server running at http://localhost:3000"));
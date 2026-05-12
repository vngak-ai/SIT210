const fetch = (...args) => import('node-fetch').then(({default: f}) => f(...args));

fetch('https://api2.arduino.cc/iot/v1/clients/token', {
  method: 'POST',
  headers: {'Content-Type': 'application/x-www-form-urlencoded'},
  body: new URLSearchParams({
    grant_type: 'client_credentials',
    client_id: 'qHY55woW0VoyVC9yyYrJVtMyMynC2gGV',
    client_secret: 'Wu3FTpgNHlXHn43gp40oIRLZrPtWlBETqM66FOh9LVcPrDwk4uKopq8hB0M0eNPi',
    audience: 'https://api2.arduino.cc/iot'
  })
}).then(r => r.json()).then(d =>
  fetch('https://api2.arduino.cc/iot/v2/things/c513a61f-eed3-4f34-a631-85edf125b783/properties', {
    headers: {'Authorization': 'Bearer ' + d.access_token}
  }).then(r => r.json()).then(props =>
    props.forEach(p => console.log(p.name, ':', p.id))
  )
);
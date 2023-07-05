const express = require("express");
const app = express();

app.use(express.json());

app.put("/api/Esp32/:espName", (req, res) => {
  console.log("Received data from ESP32:", req.params.espName);
  console.log(req.body);
  res.sendStatus(200);
});

app.get("/api/Esp32/:espName", (req, res) => {
  console.log("Received GET request from ESP32:", req.params.espName);
  res.sendStatus(200);
});

app.listen(3000, () => {
  console.log("Server is running on port 3000");
});
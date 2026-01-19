#include <WiFi.h>
#include <esp_now.h>
#include <WebServer.h>
#include <esp_wifi.h>


/* ================= WiFi ================= */
const char* ssid = "Galaxy";
const char* password = "sahmi123";

/* ================= LED PINS (ESP32-C3 SAFE) ================= */
#define LED_SAFE    9   // >20 cm
#define LED_WARN    7   // 10–20 cm
#define LED_DANGER  2   // <10 cm
#define buzzer      5



/* ================= WEB SERVER ================= */
WebServer server(80);

/* ================= ESP-NOW STRUCT ================= */
typedef struct {
  float distance;
} struct_message;

struct_message myData;
float lastDistance = 0.0;

/* ================= ESP-NOW RECEIVE ================= */
void OnDataRecv(const esp_now_recv_info *info,
                const uint8_t *incomingData,
                int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  lastDistance = myData.distance;

  Serial.print("Received distance: ");
  Serial.println(lastDistance);

  if (lastDistance > 20) {
    digitalWrite(LED_SAFE, HIGH);
    digitalWrite(LED_WARN, LOW);
    digitalWrite(LED_DANGER, LOW);
    digitalWrite(buzzer, LOW);

  } else if (lastDistance > 10) {
    digitalWrite(LED_SAFE, LOW);
    digitalWrite(LED_WARN, HIGH);
    digitalWrite(LED_DANGER, LOW);
    digitalWrite(buzzer, LOW);
    
  } else {
    digitalWrite(LED_SAFE, LOW);
    digitalWrite(LED_WARN, LOW);
    digitalWrite(LED_DANGER, HIGH);
    digitalWrite(buzzer, HIGH);
    
  }
}

/* ================= JSON DATA ================= */
void handleData() {
  String json = "{\"distance\":" + String(lastDistance, 2) + "}";
  server.send(200, "application/json", json);
}

/* ================= DASHBOARD ================= */
void handleRoot() {
  server.send(200, "text/html", R"rawliteral(
<!DOCTYPE html>
<html>
<head>
<meta charset="utf-8">
<title>Flood Monitoring Dashboard</title>

<!-- Icons -->
<link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/6.4.0/css/all.min.css">

<!-- Chart.js -->
<script src="https://cdn.jsdelivr.net/npm/chart.js"></script>

<style>
body{
  margin:0;
  font-family: 'Segoe UI', Arial, sans-serif;
  background: linear-gradient(135deg,#0f2027,#203a43,#2c5364);
  color:#fff;
}

.header{
  padding:20px;
  text-align:center;
  background:rgba(0,0,0,0.3);
  box-shadow:0 4px 10px rgba(0,0,0,0.4);
}

.container{
  max-width:900px;
  margin:30px auto;
  padding:20px;
}

.cards{
  display:grid;
  grid-template-columns:repeat(auto-fit,minmax(250px,1fr));
  gap:20px;
}

.card{
  background:rgba(255,255,255,0.1);
  border-radius:15px;
  padding:20px;
  box-shadow:0 8px 20px rgba(0,0,0,0.4);
  text-align:center;
}

.card i{
  font-size:40px;
  margin-bottom:10px;
}

.value{
  font-size:2.8rem;
  font-weight:bold;
}

.status{
  margin-top:10px;
  font-size:1.4rem;
  font-weight:bold;
}

.safe{color:#00ff88;}
.warn{color:#ffb300;}
.danger{color:#ff4c4c;}

.chart-card{
  margin-top:30px;
  background:#fff;
  border-radius:15px;
  padding:20px;
  color:#000;
}

.footer{
  text-align:center;
  padding:15px;
  opacity:0.7;
  font-size:0.9rem;
}
</style>
</head>

<body>

<div class="header">
  <h1>🌊 Flood Monitoring System</h1>
  <p>Real-time Water Level Monitoring</p>
</div>

<div class="container">

  <div class="cards">
    <div class="card">
      <i class="fa-solid fa-ruler-vertical"></i>
      <div class="value" id="distance">-- cm</div>
      <div>Water Distance</div>
    </div>

    <div class="card">
      <i class="fa-solid fa-triangle-exclamation"></i>
      <div class="status" id="zone">---</div>
      <div>Alert Status</div>
    </div>
  </div>

  <div class="chart-card">
    <h3>Distance Trend</h3>
    <canvas id="chart"></canvas>
  </div>

</div>

<div class="footer">
  ESP32-C3 | ESP-NOW | IoT Flood Monitoring
</div>

<script>
let ctx=document.getElementById('chart').getContext('2d');
let chart=new Chart(ctx,{
  type:'line',
  data:{
    labels:[],
    datasets:[{
      label:'Distance (cm)',
      data:[],
      borderColor:'#0077ff',
      backgroundColor:'rgba(0,119,255,0.2)',
      tension:0.4,
      fill:true
    }]
  },
  options:{
    responsive:true,
    scales:{
      y:{beginAtZero:true}
    }
  }
});

async function updateData(){
  let r=await fetch('/data');
  let j=await r.json();
  let d=j.distance;

  document.getElementById('distance').innerText=d.toFixed(2)+" cm";

  let z=document.getElementById('zone');
  z.className="status";

  if(d>20){
    z.innerText="SAFE";
    z.classList.add("safe");
  }else if(d>10){
    z.innerText="WARNING";
    z.classList.add("warn");
  }else{
    z.innerText="DANGER";
    z.classList.add("danger");
  }

  let t=new Date().toLocaleTimeString();
  chart.data.labels.push(t);
  chart.data.datasets[0].data.push(d);

  if(chart.data.labels.length>25){
    chart.data.labels.shift();
    chart.data.datasets[0].data.shift();
  }

  chart.update();
}

setInterval(updateData,1000);
</script>

</body>
</html>
)rawliteral");
}


/* ================= SETUP ================= */
void setup() {
  Serial.begin(115200);

  pinMode(LED_SAFE, OUTPUT);
  pinMode(LED_WARN, OUTPUT);
  pinMode(LED_DANGER, OUTPUT);
  pinMode(buzzer,OUTPUT);
  
  //pinMode(Buzzer,OUTPUT);

  /* ---- WiFi + ESP-NOW CHANNEL FIX ---- */
  WiFi.mode(WIFI_STA);

  // 🔥 LOCK CHANNEL (MUST MATCH TRANSMITTER)
  esp_wifi_set_channel(1, WIFI_SECOND_CHAN_NONE);

  // Now connect to WiFi
  WiFi.begin(ssid, password);

  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi Connected");

  Serial.print("ESP32-C3 IP Address: ");
  Serial.println(WiFi.localIP());

  /* ---- ESP-NOW ---- */
  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW Init Failed");
    return;
  }

  esp_now_register_recv_cb(OnDataRecv);
  Serial.println("ESP-NOW Receiver Ready");

  /* ---- WEB SERVER ---- */
  server.on("/", handleRoot);
  server.on("/data", handleData);
  server.begin();

  Serial.println("Web server started");
}

/* ================= LOOP ================= */
void loop() {
  server.handleClient();
}


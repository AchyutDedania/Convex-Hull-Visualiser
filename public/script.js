var points = [];
var hullPoints = [];
var lineContainer = document.getElementById('line-container');

window.addEventListener('beforeunload', function(event) {
  // Perform a fetch request to the "/empty-points" route

  fetch('/empty-points', {
      method: 'GET'
  })
  .then(response => {
      if (!response.ok) {
          throw new Error('Failed to empty points file.');
      }
      console.log('Points file emptied successfully.');
  })
  .catch(error => {
      console.error('Error emptying points file:', error);
  });
});

document.getElementById("random-points-generate").addEventListener("click",function(){
  var dots = document.querySelectorAll(".dot");
  dots.forEach(function (dot) {
    dot.parentNode.removeChild(dot);
  });
  var lines = document.querySelectorAll(".line");
  lines.forEach(function (l) {
    l.parentNode.removeChild(l);
  });
  points = [];
  hullPoints = [];
  event.preventDefault();

  const numPoints = document.getElementById("points").value;

  fetch(`/random-points-generator?points=${numPoints}`)
    .then((response) => {
      if (response.ok) {
        return;
      } 
    })
});

document.getElementById("show-points").addEventListener("click", function(event){
  event.preventDefault();
  fetch("/points.csv")
    .then((response) => response.text())
    .then((data) => {
      points = parseCSV(data);
      drawPoints();
    })
});


document.getElementById("box-main-container").addEventListener("click", function(){
  const x=event.offsetX;
  const y=event.offsetY;
  var dot = createDot(x, y);
  this.appendChild(dot);
  const sendData = async () => {
    const csvData = `${x},${y}\n`; // Your string data
    const response = await fetch('/add', {
        method: 'POST',
        headers: {
            'Content-Type': 'application/json'
        },
        body: JSON.stringify({ data: csvData })  // Ensure data is properly stringified
    });
    const responseData = await response.json();
  };
  sendData();
});


document.getElementById("run").addEventListener("click", function(event){
  event.preventDefault();
  const algorithm = document.getElementById("algorithm").value;
  fetch(`/algo?algorithm=${algorithm}`)
    .then((response) => {
      if (response.ok) {
        return;
      } 
    })
});

document.getElementById("visualize").addEventListener("click", function(event){
  event.preventDefault();
  fetch("/results/results_1.csv")
    .then((response) => response.text())
    .then((data) => {
      hullPoints = parseCSV(data);
      draw();
    })
});

document.getElementById('uploadBtn').addEventListener('click', function(event){
  event.preventDefault();
  const fileInput = document.getElementById('csvFileInput');
  const file = fileInput.files[0];

  const formData = new FormData();
  formData.append('file', file);

  fetch('/upload-csv', {
      method: 'POST',
      body: formData
  })
  .then(response => {})
  .catch(error => {});
});

document.getElementById('skipAnimation').addEventListener('click',function(event){
  event.preventDefault();
  fetch("/results/results_1.csv")
    .then((response) => response.text())
    .then((data) => {
      // Parse the CSV data
      hullPoints = parseCSV(data);
      const algo = document.getElementById("algorithm").value;
      drawHullPointswithSkip(algo);
    })
    .catch((error) => {
      console.error("Error fetching file:", error);
    });
});

function draw(){
  const algo = document.getElementById("algorithm").value;
  if (algo == "jarvis") {
        jarvis();
  } 
  else {
      kPS();
  }
}

function drawPoints() {
  var boxContainer = document.getElementById(
    "box-main-container"
  );
  for (let i = 0; i < points.length; i++) {
    var dot = createDot(points[i].x, points[i].y);
    points.pop();
    console.log(points[i].x + " " + points[i].y);
    boxContainer.appendChild(dot);
  }
}

function markpoint(pts,color)
{
  
  var pt=document.getElementById('x'+Math.abs(pts.x)+'y'+Math.abs(pts.y));
  
  pt.style.backgroundColor=color;
}

function unmarkpoint(pts)
{
  var pt=document.getElementById('x'+Math.abs(pts.x)+'y'+Math.abs(pts.y));
  pt.style.backgroundColor="black";
}

function parseCSV(csv) {
  const segment = csv.split("\n");
  const arr = [];
  segment.forEach((line) => {
    const subSegment = line.split(",");
    const x = parseFloat(subSegment[0]);
    const y = parseFloat(subSegment[1]);
    if (!isNaN(x) && !isNaN(y)) {
      arr.push({ x, y });
    } 
  });
  return arr;
}

function createDot(x, y) {
  var dot = document.createElement("div");
  dot.className = "dot";
  dot.id = "x" + x + "y" + y;
  dot.style.left = x - 5 + "px";
  dot.style.top = y - 5 + "px";
  points.push({ x, y });
  return dot;
}

function createLine(pts1,pts2,color) {
  x1=Math.abs(pts1.x);
  x2=Math.abs(pts2.x);
  y1=Math.abs(pts1.y);
  y2=Math.abs(pts2.y);
  var l = document.createElementNS("http://www.w3.org/2000/svg", "line");
    l.id="x1"+x1+"y1"+y1+"x2"+x2+"y2"+y2+"c"+color;
    l.setAttribute("x1", x1);
    l.setAttribute("y1", y1);
    l.setAttribute("x2", x2);
    l.setAttribute("y2", y2);
    l.setAttribute("stroke",color); // Adjust color as needed
    l.setAttribute("class", "line");
    lineContainer.appendChild(l);
  return l
  }


function sleep(ms) {
  return new Promise((resolve) => setTimeout(resolve, ms));
}

async function kPS(){
  for(let i=0;i<hullPoints.length;i++){
    markpoint(hullPoints[i],'red');
  }
  await sleep(1000);
  for(let j=0;j<hullPoints.length;j+=2){
    createLine(hullPoints[j],hullPoints[j+1],'blue');
    await sleep(1000);
  }
}

async function jarvis(){
  for(let i=0;i<hullPoints.length;i++){
    for(let j=0;j<points.length;j++){
      markpoint(hullPoints[i],'red');
      markpoint(points[j],'green');
      var line=createLine(hullPoints[i],points[j],'orange');
      await sleep(100);
      line.parentNode.removeChild(line);
      unmarkpoint(points[j]);
    }
    if(i==hullPoints.length-1){
      createLine(hullPoints[0],hullPoints[hullPoints.length-1],'blue');
    }
    else{
      createLine(hullPoints[i],hullPoints[i+1],'blue');
    }
    unmarkpoint(hullPoints[i]);
  }
  
}

function drawHullPointswithSkip(algo){
  if(algo=="jarvis"){
    for(let i=0;i<hullPoints.length;i++){
      markpoint(hullPoints[i],'red');
    }
    for(let j=0;j<hullPoints.length-1;j++){
      createLine(hullPoints[j],hullPoints[j+1],'blue');
    }
    createLine(hullPoints[0],hullPoints[hullPoints.length-1],'blue');
  }
  else{
    for(let i=0;i<hullPoints.length;i++){
      markpoint(hullPoints[i],'red');
    }
    for(let j=0;j<hullPoints.length-1;j+=2){
      createLine(hullPoints[j],hullPoints[j+1],'blue');
    }
  }
  
}

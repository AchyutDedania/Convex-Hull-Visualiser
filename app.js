const express = require("express");
const fs = require("fs");
const app = express();
const path = require("path");
const bodyParser = require('body-parser');

app.use(bodyParser.urlencoded({ extended: false }));
app.use(bodyParser.json());
const multer = require('multer');
const upload = multer({ dest: 'uploads/' });

app.use(express.static("public"));

// Define a route
app.get("/results/results_1.csv", (req, res) => {
  const filePath = path.join(__dirname, "results/results_1.csv");
  fs.open(filePath, "r", (err, fd) => {
    fs.readFile(filePath, "utf8", (err, data) => {
      fs.close(fd, (err) => {
        // res.set("Content-Type", "text/csv");
        // res.send(data);
      });
      res.set("Content-Type", "text/csv");
      res.send(data);
    });
  });
});

app.get("/algo", function (req, res) {
  const spawn = require("child_process").spawn;
  if (req.query.algorithm == 'jarvis') {
    const childProcess1 = spawn("g++", ["main1.cpp", "-o", "main1"]);
    childProcess1.on("close", () => {
      const childProcess2 = spawn("./main1", []);
      childProcess2.on("close", () => { });
    });
  } else {
    const childProcess1 = spawn("g++", ["main2.cpp", "-o", "main2"]);
    childProcess1.on("close", () => {
      const childProcess2 = spawn("./main2", []);
      childProcess2.on("close", () => { });
    });
  }
  res.status(200).send();
});

app.get('/empty-points', (req, res) => {
  const filePath = path.join(__dirname, 'points.csv');

  // Truncate the file to empty its contents
  fs.truncate(filePath, 0, (err) => {
      if (err) {
          console.error('Error truncating file:', err);
          res.status(500).send('Error emptying file.');
          return;
      }
      console.log('File emptied successfully.');
      res.status(200).send('File emptied successfully.');
  });
});

app.get("/points.csv", (req, res) => {
  const filePath = path.join(__dirname, "points.csv");
  fs.open(filePath, "r", (err, fd) => {
    fs.readFile(filePath, "utf8", (err, data) => {
      fs.close(fd, (err) => {
        // res.set("Content-Type", "text/csv");
        // res.send(data);
      });
      res.set("Content-Type", "text/csv");
      res.send(data);
    });
  });
});

app.post('/upload-csv', upload.single('file'), (req, res) => {
  const csvPath = req.file.path;
  const fs = require('fs');

  fs.readFile(csvPath, 'utf8', (err, data) => {
    const pointsCsvPath = path.join(__dirname, 'points.csv');
    fs.appendFile(pointsCsvPath, data, 'utf8', (err) => {
      res.status(200).send();
    });
  });
});

app.get("/random-points-generator", function (req, res) {
  let numPoints = Number(req.query.points);
  const spawn = require("child_process").spawn;
  const childProcess = spawn("python", ["make_set.py", numPoints]);
  childProcess.stdout.on('data', (data) => {
    console.log(`stdout: ${data}`);
  });
  childProcess.on("close", () => {
    res.status(200).send();
  });
});

app.get("/", function (req, res) {
  res.sendFile(__dirname + "/index.html");
});

const port = 4000;

app.listen(port, () => {
  console.log(`Listening on port 4000`);
});

app.post('/add', (req, res) => {
  const csvPath = path.join(__dirname, 'points.csv');
  const csvData = req.body.data;

  fs.appendFile(csvPath, csvData, 'utf8', (err) => {
    if (err) {
      console.error('Error appending coordinates to points.csv:', err);
      res.status(500).send('Error appending coordinates to points.csv');
      return;
    }
    console.log('Coordinates appended to points.csv');
    res.status(200).send();
  });
});
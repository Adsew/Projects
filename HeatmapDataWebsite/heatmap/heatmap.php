<?php
// Code is from:
// https://www.w3schools.com/php/php_mysql_connect.asp
// https://www.w3schools.com/php/php_mysql_select.asp

$servername = "localhost";
$username = "root";
$password = "";
$dbname = "heatmap";

// Create connection
$conn = mysqli_connect($servername, $username, $password, $dbname);

// Check connection
if (!$conn) {
    die("Connection failed: " . mysqli_connect_error() . "<br>");
}

$sqlSelect = "SELECT x, y FROM clicks";
$result = mysqli_query($conn, $sqlSelect);

$xs = array();
$ys = array();

$pointCount = mysqli_num_rows($result);

if ($pointCount > 0) {
    
	$xs = array_fill(0, $pointCount, 0);
	$ys = array_fill(0, $pointCount, 0);
	
	// fill data into variables
	$i = 0;
    while($row = mysqli_fetch_assoc($result)) {
		$xs[$i] = $row["x"];
		$ys[$i] = $row["y"];
		$i = $i + 1;
    }
} else {
    echo "0 results";
}

mysqli_close($conn);
?>

<!DOCTYPE html>
<html>
<meta charset="UTF-8" />

<body>

<h1>Heatmap by Chidozie and James.</h1>

<canvas id="heatmapColour" width="800" height="600" style="border:1px solid #000000FF;">
	Canvas not supported on this browser.
	<img id="map" src="Assignment2.png" alt="Map">
</canvas>

<script>

drawMap();

function drawMap() {
	
	var c = document.getElementById("heatmapColour");
	var ctx = c.getContext("2d");
	var img = document.getElementById("map");
	ctx.drawImage(img,0,0);
	
	drawHeat();
}

// ALL CODE HERE IS BY THE FOLLOWING TUTORIAL
// http://zhangwenli.com/blog/2015/06/12/drawing-heatmap-with-html-canvas/
function drawHeat(){
	
	// Prepare brush
	var brushCanvas = document.createElement('canvas');
	var brushSize = 10;
	var brushBlurSize = 10;

	var r = brushSize + brushBlurSize;
	var d = r * 2;
    brushCanvas.id = "brush";
	brushCanvas.width = d;
	brushCanvas.height = d;

	var ctx = brushCanvas.getContext('2d');
	ctx.shadowOffsetX = d;
	ctx.shadowBlur = brushBlurSize;
	ctx.shadowColor = 'black';

	// draw base circle for heat
	ctx.beginPath();
	ctx.arc(-r, r, brushSize, 0, Math.PI * 2, true);
    ctx.closePath();
	ctx.fill();

	// Draw random data on map
	var mapCan = document.getElementById("heatmapColour");
	mapctx = mapCan.getContext("2d");
	
    var heatCan = document.createElement("canvas");
	heatCan.width = mapCan.width;
	heatCan.height = mapCan.height;
    var heatctx = heatCan.getContext("2d");
    
	var count = "<?php echo $pointCount ?>";
	var xCoords = <?php echo json_encode($xs) ?>;
	var yCoords = <?php echo json_encode($ys) ?>;
	
	var data = [];
	
	for (var i = 0; i < count; ++i) {
		data.push([xCoords[i], yCoords[i], 0.2]);
	}
	
	var len = data.length;
	for (var i = 0; i < len; ++i) {
		var p = data[i];
		var x = p[0];
		var y = p[1];
		var alpha = p[2]; // using value as alpha

		// draw with the circle brush with alpha
		heatctx.globalAlpha = alpha;
		heatctx.drawImage(brushCanvas, x - r, y - r);
	}

	// Prepare gradient for colour
	var levels = 256;
	var gradientCan = document.createElement('canvas');
	gradientCan.width = 1;
	gradientCan.height = levels;
	var gradientctx = gradientCan.getContext('2d');

	var gradientColors = {
		0.4: 'blue',
		0.5: 'cyan',
		0.6: 'lime',
		0.8: 'yellow',
		1.0: 'red'
	};

	// add color to gradient stops
	var gradient = gradientctx.createLinearGradient(0, 0, 0, levels);
	for (var pos in gradientColors) {
		gradient.addColorStop(pos, gradientColors[pos]);
	}

	gradientctx.fillStyle = gradient;
	gradientctx.fillRect(0, 0, 1, levels);
	var gradientPixels = gradientctx.getImageData(0, 0, 1, levels).data;
	
	// Get heat and colourize it
	var imageData = heatctx.getImageData(0, 0, heatCan.width, heatCan.height);
	var pixels = imageData.data;
	var len = pixels.length / 4;
	while(len--) {
		var id = len * 4 + 3;
		var alpha = pixels[id] / 256; // why not `gradientLevels`?

		var colorOffset = Math.floor(alpha * (levels - 1));
		pixels[id - 3] = gradientPixels[colorOffset * 4];     // red

		pixels[id - 2] = gradientPixels[colorOffset * 4 + 1]; // green

		pixels[id - 1] = gradientPixels[colorOffset * 4 + 2]; // blue

	}
	
	heatctx.putImageData(imageData, 0, 0);
	mapctx.globalAlpha = 0.85;
	mapctx.drawImage(heatCan, 0, 0);
}
</script>

</body>
</html>
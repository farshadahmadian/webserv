// "use strict";

// while (true) {}

const html = `<!DOCTYPE html>
<html lang="en">
<head>
	<meta charset="UTF-8" />
	<meta name="viewport" content="width=device-width, initial-scale=1.0" />
	<title>Document</title>
	<style>
		h1 {
			display: flex;
			justify-content: center;
			align-items: center;
			color: blue;
		}
	</style>
</head>
<body>
<h1>Welcome ${process.env.NAME}</h1>
<p>Time: </p>
</body>
</html>`;
console.log(html);

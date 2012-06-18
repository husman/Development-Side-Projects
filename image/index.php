<?php

require_once 'class/class.Image.php';
$image = new Image("example.jpg");

$image->handler->resize(500,500)->save("example1.jpg");

$image->handler->rotate(45, 0xFFFFFF)->save("example2.jpg");

$image->handler->scale(3, 3)->save("example3.jpg");

?>

<html>
<head>
<title>Example Usage</title>
</head>
<body>

<h3>Example #1</h3>
<img alt="example1.jpg" src="example1.jpg" />
<br />

<h3>Example #2</h3>
<img alt="example2.jpg" src="example2.jpg" />
<br />

<h3>Example #3</h3>
<img alt="example3.jpg" src="example3.jpg" />
<br /><br />


More coming soon! This was created very quickly for demostration purposes...
<br /><br />

</body>
</html>
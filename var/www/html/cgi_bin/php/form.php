<?php

echo "<html>";
echo "<head>";
echo "<title>Upload form response</title>";
echo "</head>";
echo "<body>";

if (!empty($_POST))
{
	echo ("<h1>input1: " . htmlspecialchars($_POST["text1"]) . "</h1><br>
			<h1>text2: " . htmlspecialchars($_POST["text2"]) . "</h1>");
}
else
{
	echo "<h1>\$_POST is empty.</h1><br>";
}

// UPLOAD
if ($_SERVER["UPLOAD_PATH"] && $_SERVER["ROOT_PATH"])
	$uploaddir = $_SERVER["ROOT_PATH"] . $_SERVER["UPLOAD_PATH"];
else
	$uploaddir = __DIR__ . '/upload/';
if (!file_exists($uploaddir))
	mkdir($uploaddir, 0700);
$uploadfile = $uploaddir . basename($_FILES['userfile']['name']);
$tmp_name = $_FILES['userfile']['tmp_name'];
echo "<br>File: " . basename($_FILES['userfile']['name']) . "<br>";
echo "Upload path: " . $uploaddir . "<br><br>";
if (move_uploaded_file($tmp_name, $uploadfile))
{
	echo "File uploaded successfully!<br>";
}
else
{
	echo "Couldn't upload the file.<br>";
	echo "<br>kaas" . $tmp_name . "kaas<br>";
	echo "<br>" . $uploadfile . "<br>";
}

echo "</body>";
echo "</html>";
?>
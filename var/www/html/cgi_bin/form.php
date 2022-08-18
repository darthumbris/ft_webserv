<?php

if (!empty($_POST))
{
	echo ("text1: " . htmlspecialchars($_POST["text1"]) . "<br>
			text2: " . htmlspecialchars($_POST["text2"]));
}
else
{
	echo "\$_POST is empty<br>";
}

// UPLOAD
if ($_SERVER["UPLOAD_PATH"])
	$uploaddir = __DIR__ . $_SERVER["UPLOAD_PATH"];
else
	$uploaddir = __DIR__ . '/upload/';
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
}
?>
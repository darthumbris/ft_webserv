<?php

if (!empty($_POST))
{
	echo ("Input:" . "\n" . "text1:" . $_POST["text1"] . " text2: " . htmlspecialchars($_POST["text2"]) . "!\n");
}
else
{
	echo "\$_POST is empty\n";
}
echo "\n";
// UPLOAD
$uploaddir = __DIR__ . '/upload/';
$uploadfile = $uploaddir . basename($_FILES['userfile']['name']);
$tmp_name = $_FILES['userfile']['tmp_name'];
if (move_uploaded_file($tmp_name, $uploadfile)) {
	echo "File " . $uploadfile . " uploaded successfully\n";
} else {
	echo "Couldn't upload the file.\n";
}
?>
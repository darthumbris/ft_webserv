<html>
<body>

<?php
if (isset($_POST['name'], $_POST['email']))
{
    echo "Welcome " ;
    echo $_POST["name"];
    echo "<br>";
    echo "Your email adress is: ";
    echo $_POST["email"];
}
else
{
    echo "Welcome name_not_found <br>";
    echo "Your emaild adress is: email_not_found";
}
?>
</body>
</html>
<?php

$servername    = "localhost";
$dbname        = "watter_bd";
$tbname        = "watter_tb";
$username      = "watter_user";
$password      = "******";
$api_key_value = "******";

$api_key = $user_id = $user_location = $cold = $hot = $alarm_time = $Time = "";

if ($_SERVER["REQUEST_METHOD"] == "POST") {
    $api_key = test_input($_POST["api_key"]);
    if($api_key == $api_key_value) {
        //$user_id       = test_input($_POST["user_id"]);
        //$user_location = test_input($_POST["user_location"]);
        //$cold          = test_input($_POST["cold"]);
        //$hot           = test_input($_POST["hot"]);
        //$alarm_time    = test_input($_POST["alarm_time"]);

		$conn = pg_connect("host=$servername dbname=$dbname user=$username password=$password");
		if (!$conn) {
			die("Connection failed: " . $conn->connect_error);
		}

		$sql = "DELETE FROM $tbname";
		$result = pg_query($sql);

		if (!$result) {
			die("Clear failed ");
		} else {
			die("Watter clear successfully");
		}
	}
	else {
        echo "Wrong API Key provided.";
    }
}

function test_input($data) {
    $data = trim($data);
    $data = stripslashes($data);
    $data = htmlspecialchars($data);
    return $data;
}

?>

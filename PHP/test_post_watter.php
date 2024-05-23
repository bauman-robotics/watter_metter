<?php
header("Content-Type: text/plain; charset=utf-8");

$servername = "localhost";
$dbname     = "watter_bd";
$tbname     = "watter_tb";
$username   = "watter_user";
$password   = "******";

$api_key = $user_id = $user_location = $cold = $hot = $alarm_time = $Time = "";

$line_num      = 1;
$user_id       = "NoName";
$user_location = "NoPlace";
$cold          = 1;
$hot           = 2;
$alarm_time    = 1940;

$conn = pg_connect("host=$servername dbname=$dbname user=$username password=$password");
if (!$conn) {
	die("Connection failed " . $conn->connect_error);
}
$sql = "INSERT INTO $tbname (line_num, user_id, user_location, cold, hot, alarm_time)
VALUES (DEFAULT, '" . $user_id . "', '" . $user_location . "', '" . $cold . "', '" . $hot . "', '" . $alarm_time . "')";

$result = pg_query($sql);

if (!$result) {
	die("Insert failed " . $conn->connect_error);
} else {
	die("New record created successfully");
}

?>

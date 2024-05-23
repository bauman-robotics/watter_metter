<?php

header("Content-Type: text/plain; charset=utf-8");

$servername = "localhost";
$dbname     = "watter_bd";
$tbname     = "watter_tb";
$username   = "watter_user";
$password   = "******";

$conn = pg_connect("host=$servername dbname=$dbname user=$username password=$password");
if (!$conn) {
	die("Connection failed: " . $conn->connect_error);
}

$sql = "DELETE FROM $tbname";

$result = pg_query($sql);

if (!$result) {
	die("Clear failed \n");
} else {
	die("Watter clear successfully \n");
}

?>

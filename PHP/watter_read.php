<!DOCTYPE html>
<html><body>
<?php
/*
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp32-esp8266-mysql-database-php/

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
 
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*/

$servername = "localhost";

$dbname   = "watter_bd";
$username = "watter_user";
$password = "******";

$conn = pg_connect("host=localhost dbname=$dbname user=$username password=$password");
if (!$conn) {
    die("Connection failed: " . $conn->connect_error);
}

$sql = "SELECT line_num,  user_id, user_location, cold, hot, alarm_time, created_at FROM watter_tb";

echo '<table cellspacing="5" cellpadding="5">
      <tr>
        <td>line_num</td>
        <td>user_id</td>
        <td>user_location</td>
        <td>cold</td>
        <td>hot</td>
        <td>alarm_time</td>
        <td>Time</td>
      </tr>';

if ($result = pg_query($sql)) {
    while ($row = pg_fetch_assoc($result)) {
        $line_num = $row["line_num"];
	$row_id = $row["user_id"];
        $row_location = $row["user_location"];
        $row_cold = $row["cold"];
        $row_hot = $row["hot"];
        $row_alarm_time = $row["alarm_time"];
        $row_reading_time = $row["created_at"];

        $row_reading_time = date("Y-m-d H:i:s", strtotime("$row_reading_time + 4 hours"));

        echo '<tr>
		<td>' . $line_num . '</td>
                <td>' . $row_id . '</td>
                <td>' . $row_location . '</td>
                <td>' . $row_cold . '</td>
                <td>' . $row_hot . '</td>
                <td>' . $row_alarm_time . '</td>
                <td>' . $row_reading_time . '</td>
              </tr>';
    }
	pg_free_result($result);
}
pg_close($conn);
?>
</table>
</body>

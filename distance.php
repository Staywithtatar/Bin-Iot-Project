<?php

// ตั้งค่าสำหรับการเชื่อมต่อฐานข้อมูล MySQL
$servername = "localhost"; // เปลี่ยนเป็น hostname ของ MySQL server ของคุณ
$username = "root"; // เปลี่ยนเป็นชื่อผู้ใช้ของ MySQL
$password = ""; // เปลี่ยนเป็นรหัสผ่านของ MySQL
$dbname = "trash_can_status"; // เปลี่ยนเป็นชื่อฐานข้อมูลที่คุณต้องการเก็บข้อมูล

// รับค่าที่ส่งมาจาก ESP32
$lidOpen = $_POST['lidOpen']; // รับค่าสถานะการเปิดหรือปิดของฝาถังขยะ

// เชื่อมต่อฐานข้อมูล
$conn = new mysqli($servername, $username, $password, $dbname);

// เช็คการเชื่อมต่อ
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
}

// เพิ่มข้อมูลลงในฐานข้อมูล
$sql = "INSERT INTO bin (lid_open) VALUES ('$lidOpen')";

if ($conn->query($sql) === TRUE) {
    echo "Record added successfully";
} else {
    echo "Error: " . $sql . "<br>" . $conn->error;
}

$conn->close();


?>

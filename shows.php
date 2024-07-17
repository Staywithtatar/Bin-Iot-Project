<html>
<head>
    <meta http-equiv="refresh" content="3">
    <style>
        body {
            font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
            background-color: #f5f5f5;
            color: #333;
            margin: 0;
            padding: 0;
            text-align: center;
        }

        table {
            width: 50%;
            margin: 20px auto;
            border-collapse: collapse;
            background-color: #fff;
            box-shadow: 0 0 20px rgba(0, 0, 0, 0.1);
        }

        th, td {
            padding: 12px 15px;
            text-align: center;
            border-bottom: 1px solid #ddd;
        }

        th {
            background-color: #4CAF50;
            color: white;
        }

        tr:nth-child(even) {
            background-color: #f2f2f2;
        }

        tr:hover {
            background-color: #e6e6e6;
        }
    </style>
</head>
<body>
<div class="container">
        <h1>Trash Can Status</h1>
	<?php
    $servername = "localhost";
    $username = "root";
    $password = "";
    $dbname = "trash_can_status";

    // Create connection
    $conn = new mysqli($servername, $username, $password, $dbname);

    // Check connection
    if ($conn->connect_error) {
        die("Connection failed: " . $conn->connect_error);
    }

    $sql = "SELECT * FROM Bin ORDER by id DESC LIMIT 5";
    $result = $conn->query($sql);

    if ($result->num_rows > 0) {
        echo "<table><tr><th>ID</th><th>TRASH_STATUS</th><th>DATETIME</th></tr>";
        while($row = $result->fetch_assoc()) {
            echo "<tr>";
            echo "<td>".$row['id']."</td>";
            echo "<td>";
        // แปลงค่า lid_open เป็นข้อความที่เข้าใจง่าย
        if ($row["lid_open"] == 0) {
            echo "ปิด";
        } else {
            echo "เปิด";
        }
        echo "</td>";
            echo "<td>".$row['day_Time']."</td>";
            echo "</tr>";
        }
        echo "</table>";
    } else {
        echo "0 results";
    }

    $conn->close();
    ?>
	</div>
</body>
</html>
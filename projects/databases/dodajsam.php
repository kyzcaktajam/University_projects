<!DOCTYPE HTML>
<html lang = "pl">
<head>
    <meta charset = "utf-8"/>
    <title> Dodaj samochód</title>
</head>
<body>
<?php
    $marka = $_POST["marka"];
    $wartosc = $_POST["wartosc"];
    require_once "connect.php";
    $polaczenie = @new mysqli($host, $db_user, $db_password, $db_name);
    
    $sql = "SELECT * FROM samochod";
    $res = $polaczenie->query($sql);
    $row_count = mysqli_num_rows($res);
    $new_id = $row_count + 1;
    
    $que = "INSERT INTO samochod VALUES('$new_id', '$marka', '$wartosc')";
    $wyn = $polaczenie->query($que);
    echo "Pomyślnie dodano nowy samochód do bazy, jego id: $new_id";
?>
</body>
</html>




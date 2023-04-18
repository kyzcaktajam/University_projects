<!DOCTYPE HTML>
<html lang = "pl">
<head>
    <meta charset = "utf-8"/>
    <title> Dodaj usterkę</title>
</head>
<body>
<?php
    $czesc = $_POST["czesc"];
    $zniszczenie = $_POST["zniszczenie"];
    $koszt = $_POST["koszt"];
    $id_sam = $_POST["id_sam"];
    require_once "connect.php";
    $polaczenie = @new mysqli($host, $db_user, $db_password, $db_name);
    
    $sql = "SELECT * FROM usterka";
    $res = $polaczenie->query($sql);
    $row_count = mysqli_num_rows($res);
    $new_id = $row_count + 1;
    
    $qry = "SELECT * FROM usterka WHERE czesc = '$czesc' AND sam = '$id_sam'";
    $r = $polaczenie->query($qry);
    if (mysqli_num_rows($r) > 0) {
        echo "Usterka tej części tego samochodu jest już zarejestrowana w bazie";
        exit();
    }
    
    
    $que = "INSERT INTO usterka VALUES('$new_id', '$czesc', '$zniszczenie', '$koszt', '$id_sam')";
    $wyn = $polaczenie->query($que);
    echo "Pomyślnie dodano nową usterkę do bazy, jej id: $new_id";
?>
</body>
</html>




<!DOCTYPE HTML>
<html lang = "pl">
<head>
    <meta charset = "utf-8"/>
    <title> Samochody</title>
</head>
<body>
<?php
  require_once "connect.php";
  $polaczenie = @new mysqli($host, $db_user, $db_password, $db_name);
  if ($polaczenie->connect_errno != 0) {
      echo "ERROR";
  }
  else {
      $sql = "SELECT * FROM samochod";
      $res = $polaczenie->query($sql);
      
      echo "Id   Marka   Wartość<br/><br/>";
      while ($row = mysqli_fetch_array($res)) {
          $id = $row["id_sam"];
          $marka = $row["marka"];
          $wart = $row["wartosc"];
          echo "$id   $marka $wart <br/><br/>";
      }
      $polaczenie->close();
  }
?>
</body>
</html>


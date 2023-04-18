<!DOCTYPE HTML>
<html lang = "pl">
<head>
    <meta charset = "utf-8"/>
    <title> Mechanicy</title>
</head>
<body>
<?php
  require_once "connect.php";
  $polaczenie = @new mysqli($host, $db_user, $db_password, $db_name);
  if ($polaczenie->connect_errno != 0) {
      echo "ERROR";
  }
  else {
      $sql = "SELECT * FROM mechanik";
      $res = $polaczenie->query($sql);
      
      echo "Imię   Nazwisko    Poziom umiejętności<br/><br/>";
      while ($row = mysqli_fetch_array($res)) {
          $im = $row["imie"];
          $na = $row["nazwisko"];
          $p = $row["kompetencje"];
          echo "$im $na $p<br/><br/>";
      }
      $polaczenie->close();
  }
?>
</body>
</html>


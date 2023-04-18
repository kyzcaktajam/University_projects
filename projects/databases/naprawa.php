<!DOCTYPE HTML>
<html lang = "pl">
<head>
    <meta charset = "utf-8"/>
    <title> Usterka</title>
</head>
<body>
<?php
  require_once "connect.php";
  $id_samochodu = $_POST["id_sam"];
  $polaczenie = @new mysqli($host, $db_user, $db_password, $db_name);
  if ($polaczenie->connect_errno != 0) {
      echo "ERROR";
  }
  else {
      $sql = "SELECT * FROM usterka WHERE sam = '$id_samochodu'";
      $res = $polaczenie->query($sql);
      $rowcount = mysqli_num_rows($res);
      if ($rowcount == 0) {
          echo "Brak usterek";
      }
      else {
          $laczny_koszt = 0;
          while ($row = mysqli_fetch_array($res)) {
              echo "Uszkodzona część: ".$row["czesc"]."<br/>";
              echo "Koszt naprawy : ".$row["koszt_naprawy"]."zł <br/>";
              $nc = $row["czesc"];
              $pz = $row["procent_zniszczenia"];
              
              $laczny_koszt = $laczny_koszt + $row["koszt_naprawy"];
              
              $que = "SELECT mechanik.imie, mechanik.nazwisko FROM mechanik, umiejetnosc WHERE mechanik.id_mech = umiejetnosc.mech AND umiejetnosc.czesc = '$nc' AND mechanik.kompetencje >= '$pz'";
              $wyn = $polaczenie->query($que);
              $rc = mysqli_num_rows($wyn);
              if ($rc == 0) {
                  echo "Niestety żaden z mechaników nie jest w stanie naprawić tej usterki.";
              }
              else {
                  echo "Mechanicy zdolni do naprawy usterki: <br/>";
                  while ($r = mysqli_fetch_array($wyn)) {
                      $imie = $r["imie"];
                      $nazw = $r["nazwisko"];
                      echo $imie." ".$nazw."<br/>";
                  }
              }
              echo "<br/><br/>";
          }
          echo "Łączny koszt naprawy: ".$laczny_koszt."zł.";
      }
      $polaczenie->close();
  }
?>
</body>
</html>


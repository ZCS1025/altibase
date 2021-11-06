<?php
error_reporting(E_ALL);
ini_set("display_errors",1);

$db = new PDO("altibase:Server=127.0.0.1;Port=20600", "sys", "manager");

if ($db) {

    // direct-execution
    echo "now, i create table t1 (id integer, name char(20)";
    $stmt = $db->prepare("drop table t1" );
    $stmt->execute();
    $stmt = $db->prepare("create table t1 (id integer, name char(20))" );
    $stmt->execute();

    // prepare-execution
    echo "now, i insert into t1 values";
    $emps = array("Jay", "King", "Tom", "Peter", "Newton");
    for($x =0;$x <=4;$x++) {
        $stmt = $db->prepare("insert into t1 values (?,?)");
        $stmt->bindParam(1, $x);
        $stmt->bindParam(2, $emps[$x]);
        $stmt->execute();
    }

    foreach ($db->query("select id, name from t1") as $row) {
        print("$row[ID], $row[NAME] \n");
    }
}
else
{
  echo "connection failed ...";
}
?>


<?php

require_once('FileDB.php');

$db = new FileDB();

$db->createNode('testNode/test2/test3/test4');

$db->createNode('testNode/test2/test3/test5');
$db->createNode('testNode/test2/test3/test6');
$db->createNode('testNode/test2/test3/test7');
$db->createNode('testNode/test2/test3/test8');

$c = $db->getNodeChildren('/');
var_dump($c);

?>
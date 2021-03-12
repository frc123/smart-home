<?php

$input = file_get_contents("php://input");
//TODO: put your server api url here
$address = 'http://127.0.0.1:12345/smart-home-api';
$address = $address.'?'.$_SERVER['QUERY_STRING'];
$opts = array('http' =>
    array(
        'Connection'  => 'close'
    )
);
$context = stream_context_create($opts);
$res = file_get_contents($address, false, $context);
header('HTTP/1.1 200 OK');
echo $res;

?>

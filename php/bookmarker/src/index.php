<?php

session_start();

// Simple router
$controller = ucfirst('index'); //ucfirst($_GET['c']);
$action = 'index'; //$_GET['a'];

if(!isset($_SESSION['user'])) {
    $controller = 'Login';
    $action = 'index';
}

$controller_path = 'controllers/'.$controller.'Controller.php';
$controller_class = $controller.'Controller';
include $controller_path;

$controller_cls = new $controller_class();
$controller_cls->{$action}();

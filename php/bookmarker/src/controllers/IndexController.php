<?php
include 'controllers/base/Controller.php';
include 'models/User.php';

class IndexController extends Controller {
    public function index() {
        $this->render('bookmark_list', array());
    }
}
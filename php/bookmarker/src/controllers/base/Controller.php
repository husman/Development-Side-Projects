<?php

class Controller {
    protected function render($view, $data) {
        extract($data);

        ob_start();
        include 'views/bookmark_list.php';
        echo ob_get_clean();
    }
}
<?php

echo "_SERVER VARIABLES:\n";
foreach ($_SERVER as $key => $value) {
	echo "_SERVER[$key] = $value\n";
}

echo "\n_GET VARIABLES:\n";
foreach ($_GET as $key => $value) {
	echo "_GET[$key] = $value\n";
}

echo "\n_POST VARIABLES:\n";
foreach ($_POST as $key => $value) {
	echo "_POST[$key] = $value\n";
}

?>
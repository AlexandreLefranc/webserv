<?php
if ($_SERVER['REQUEST_METHOD'] === 'POST') {
  // Retrieve the data from the request body
  $name = $_POST['name'];
  $age = $_POST['age'];

  // Process the data
  echo "Hello, $name! You are $age years old.";
}
?>
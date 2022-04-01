<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Document</title>
</head>
<body>
    <?php
    $ip = "192.168.137.16";
    if($_SERVER['REQUEST_METHOD'] == "POST") {
        if(isset($_POST['fwd'])) {
            $request = "1";
        }
        if(isset($_POST['rev'])) {
            $request = "2";
        }
        if(isset($_POST['left'])) {
            $request = "3";
        }
        if(isset($_POST['right'])) {
            $request = "4";
        }
        if(isset($_POST['line'])) {
            $request = "5";
        }
        if(isset($_POST['stop'])) {
            $request = "6";
        }
        if(isset($_POST['race'])) {
            $request = "7";
        }
        if(isset($_POST['maze'])) {
            $request = "8";
        }
        if(isset($request)) {
            $url = curl_init("http://" . $ip . "/" . $request);
            curl_exec($url);
            curl_close($url);
        }
    }
    ?> 
    <form action="#" method="post">
        <button type="submit" name="fwd" value="fwd">
            <p>forward</p>
        </button>
        <button type="submit" name="rev" value="rev">
            <p>backward</p>
        </button>
        <button type="submit" name="left" value="left">
            <p>left</p>
        </button>
        <button type="submit" name="right" value="right">
            <p>right</p>
        </button>
        <button type="submit" name="line" value="line">
            <p>linetracking</p>
        </button>
        <button type="submit" name="race" value="race">
            <p>race</p>
        </button>
        <button type="submit" name="maze" value="maze">
            <p>maze</p>
        </button>
        <button type="submit" name="stop" value="stop">
            <p>stop</p>
        </button>
    </form>
</body>
</html>
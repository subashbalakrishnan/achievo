<html>
<head>
<title>Generate Report</title>
</head>
<body>
<form name="report" action="report2.php" method="post">
<table>
<tr><td colspan=3>Generate Timesheet Report</td></tr>
<tr><td colspan=3><br></td></tr>
<tr><td>Start Date</td><td><input type="text" name="startdate" value="<?php echo date('Y-m-d', mktime(0, 0, 0, date("m")  , date("d")-7, date("Y")));?>"></td><td>(yyyy-mm-dd)</td></tr>
<tr><td>End Date</td><td><input type="text" name="enddate"  value="<?php echo date('Y-m-d');?>"></td><td>(yyyy-mm-dd)</td></tr>
<tr><td></td><td><input type="submit" name="Generate" value="Generate"></td></tr>
</table>
</form>
</body>
</html>


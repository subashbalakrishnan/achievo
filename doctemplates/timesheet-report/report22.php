<?php
//documentation on the spreadsheet package is at:
//http://pear.php.net/manual/en/package.fileformats.spreadsheet-excel-writer.php
chdir('phpxls');
require_once 'Writer.php';
chdir('..');

extract($_POST);

$query= 'SELECT concat(firstname," ",lastname) as name, GROUP_CONCAT(hours.remark SEPARATOR "\n") as remarks FROM hours join person where person.id=hours.userid and activitydate between "'.$startdate.'" and "'.$enddate.'" GROUP BY person.userid';

//echo $query;


$link = mysql_connect('localhost', 'iqss_iqssin', 'iq$$co!n');
if (!$link) {
    die('Could not connect: ' . mysql_error());
}
//echo 'Connected successfully';

mysql_select_db ('iqss_india');

$result = mysql_query ($query);

$sheet1[]=array("Name","Description");

if ($row = mysql_fetch_array($result)) {

do {
	$sheet1[]=array($row["name"],$row["remarks"]);

} while($row = mysql_fetch_array($result));

} else {print "Sorry, no records were found!";
}

mysql_close($link);

$workbook = new Spreadsheet_Excel_Writer();
$workbook->setVersion(8); 

$format_und =& $workbook->addFormat();
$format_und->setBottom(2);//thick
$format_und->setBold();
$format_und->setColor('black');
$format_und->setFontFamily('Arial');
$format_und->setSize(8);


$format_reg =& $workbook->addFormat();
$format_reg->setVAlign('top');
$format_reg->setColor('black');
$format_reg->setFontFamily('Arial');
$format_reg->setSize(8);
$format_reg->setTextWrap();

$arr = array(
      'Timesheet'=>$sheet1,     
      );

foreach($arr as $wbname=>$rows)
{
    $rowcount = count($rows);
    $colcount = count($rows[0]);

    $worksheet =& $workbook->addWorksheet($wbname);

    $worksheet->setColumn(0,0, 25.00);//setColumn(startcol,endcol,float)
    $worksheet->setColumn(1,3,100.00);
 
    
    for( $j=0; $j<$rowcount; $j++ )
    {
        for($i=0; $i<$colcount;$i++)
        {
            $fmt  =& $format_reg;
            if ($j==0)
                $fmt =& $format_und;

            if (isset($rows[$j][$i]))
            {
                
                $data=$rows[$j][$i];
               $worksheet->write($j, $i, $data, $fmt);
            }
                
        }
    }
}

$workbook->send('Timesheet_report.xls');
$workbook->close();

//-----------------------------------------------------------------------------
?>


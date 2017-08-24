<?php

useattrib("atknumberattribute");
useattrib("atktextattribute");
useattrib("atkboolattribute");
useattrib("atkdateattribute");
useattrib("atklistattribute");
useattrib("atkcreatedbyattribute");
useattrib("atkCreateStampAttribute");
useattrib("atkupdatedbyattribute");
useattrib("atkupdatestampattribute");
userelation("atkmanytoonerelation");

class leave extends atkNode{

function leave(){

$this->atkNode("leave");

$this->add(new atkAttribute("id", AF_PRIMARY|AF_HIDE|AF_AUTO_INCREMENT));  
$this->add(new atkManyToOneRelation("type","leave.leave_type",AF_OBLIGATORY|AF_SEARCHABLE));    

$this->add(new atkDateAttribute("from_date","","","",0,AF_OBLIGATORY|AF_SEARCHABLE));
$this->add(new atkDateAttribute("to_date","","","",0,AF_OBLIGATORY));

//$this->add(new atkDateAttribute("to_date","","","",0,AF_OBLIGATORY));
//$this->add(new atkDateAttribute("to_date","","",date("Ymd"),0,AF_OBLIGATORY));
$this->add(new atkRadioAttribute("duration",array("Full day","Half day"),array("Full day","Half day"),AF_OBLIGATORY|AF_HIDE_VIEW));
$this->add(new atkNumberAttribute("nod",AF_HIDE,3,1));
$this->add(new atkTextAttribute("reason",array('rows'=>2,'cols'=>40),AF_OBLIGATORY));

$this->add(new atkListAttribute("status",array('pending','approved','rejected','cancelled'),AF_HIDE_ADD|AF_HIDE_EDIT|AF_SEARCHABLE));
$this->add(new atkBoolAttribute("cancel", AF_HIDE_ADD));
$this->add(new atkTextAttribute("cancel_reason",array('rows'=>2,'cols'=>40),AF_HIDE_ADD));  
$this->add(new atkCreatedByAttribute("created_by"),"history");
$this->add(new atkCreateStampAttribute("created_on",AF_HIDE_LIST),"history");
$this->add(new atkUpdatedByAttribute("last_modified_by",AF_HIDE_LIST),"history");
$this->add(new atkUpdateStampAttribute("last_modified_on",AF_HIDE_LIST),"history");       
$this->add(new atkNumberAttribute("supervisor",AF_HIDE)); 
$this->add(new atkNumberAttribute("old",AF_HIDE));

$user = getUser();
if(date('m') < 3)
$date = date('Y-m-d', mktime(0, 0, 0, 3, 31,  date('Y')-1));
else
$date = date('Y-m-d', mktime(0, 0, 0, 3, 31,  date('Y')));

$this->addFilter('leave.created_by='.$user[id]);
$this->setTable("leave");
$this->setOrder("from_date desc");
}

function initial_values()
{
$user = getUser();
$initial = array();
$initial["status"] = 'pending';
if($user[level]==5)
$initial["created_by"] = $user['id'];
//$initial["supervisor"] = $user['supervisor'];
$initial["duration"] = 'Full day';
return $initial;
}

function action_edit(&$handler){

$leave = &atkGetNode("leave.leave");
$where = $this->m_postvars[atkselector];
$leaves = $leave->selectDb($where);
$today  = date('Ymd'); 
$fdate =  implode('',$leaves[0]['from_date']);
$fdate =  implode('',$leaves[0]['to_date']);

if($leaves[0]['status']=='approved' || $leaves[0]['status']=='cancelled'){

$attr = &$this->getAttribute("created_by");
$attr->addFlag(AF_READONLY);
$attr = &$this->getAttribute("type");
$attr->addFlag(AF_READONLY);
$attr = &$this->getAttribute("from_date");
$attr->addFlag(AF_READONLY);
$attr = &$this->getAttribute("to_date");
$attr->addFlag(AF_READONLY);
$attr = &$this->getAttribute("duration");
$attr->addFlag(AF_READONLY);
$attr = &$this->getAttribute("reason");
$attr->addFlag(AF_READONLY);
$attr = &$this->getAttribute("cancel");
$attr->addFlag(AF_READONLY);
$attr = &$this->getAttribute("cancel_reason");
$attr->addFlag(AF_READONLY);

}

return $handler->action_edit();
}

  function getFormButtons($mode, $record)
  {
    // Get the normal buttons
    $buttons = parent::getFormButtons($mode,$record);
	
    if ($mode==='edit' && ($record[status]=='cancelled' || $record[status]=='approved'))
    {
      // If the user is editting, add a 'saveandsayhello' button 
      // (you should also add a translation to your language file for saveandsayhello)
      array_shift($buttons);array_shift($buttons);
    }
    return $buttons;
  }
  
/*function action_admin(&$handler){
$page = &$this->getPage();
$user = getUser();
$this->addFilter("leave.created_by",$user['id']);
$res[] = $handler->invoke("addPage", $handler->getRejectInfo());
$res[] = $this->leaveBalance($user['id']);
$res[] = $handler->invoke("adminPage");

$page->addContent($this->renderActionPage("admin", $res));
}*/

function adminHeader()
{    
   
	$user = getUser();

	$tabledata = array_values($this->leaveCal($user['id']));
	array_unshift($tabledata,array("Leave Type","Allowed","Taken","Balance"));
    $result = "<strong>Leave Summary </strong><br><br>"; 
	//$result .= "Carry Forwarded Sick Leave : ".$user['sl_balance']." , Earned Leave : ".$user['el_balance']." <br><br>"; 
	$tablerenderer = &atknew("atk.utils.atktablerenderer");
	$result .= $tablerenderer->render($tabledata, TBL_DATA, "recordlist");
	return $result;
}

function leaveBalance($user_id,$name=''){ 

$tabledata = array_values($this->leaveCal($user_id));
array_unshift($tabledata,array("Leave Type","Allowed","Taken","Balance"));

$tablerenderer = &atknew("atk.utils.atktablerenderer");
$result = $tablerenderer->render($tabledata, TBL_DATA, "recordlist");
$ui = &atknew("atk.ui.atkui");
return $ui->renderBox(array("title"=>"Leave Summary - ".$name,"content"=>$result));   
}

function leaveBalanceHR($user_id,$name=''){   
  
$tabledata = $this->leaveCal($user_id);
$tabledata[0] = array("Leave Type","Allowed","Taken","Balance");
$tablerenderer = &atknew("atk.utils.atktablerenderer");
$result = $tablerenderer->render($tabledata, TBL_DATA, "recordlist");

return $result;   
}

function leaveCal($user_id){
$employee = &atkGetNode("employee.employee");
$user = $employee->selectDb("person.id=$user_id");
$user = $user[0];
$leave_bal = $this->leaveTypes($user);

   $db = &atkGetDb();
   $sql = "SELECT el_balance FROM person WHERE id = '".$user_id."'";
   $rows = $db->getrows($sql);
   for($i=0;$i<count($leave_bal);$i++){
     if($leave_bal[$i][name]=="Paid Leave")
      $leave_bal[$i][no] += $rows[0][el_balance];
   }

$leave_taken = $this->leaveUsed($user_id);   

$idx=0;
for($i=0;$i<count($leave_bal);$i++){
 $tabledata[$leave_bal[$i][id]] = array($leave_bal[$i][name],$leave_bal[$i][no],0,$leave_bal[$i][no]);

	for($j=0;$j<count($leave_taken);$j++){     
	   if($leave_bal[$i][id]==$leave_taken[$j][type]){
	     $tabledata[$leave_bal[$i][id]][2]  = $leave_taken[$j][taken];        
	     $tabledata[$leave_bal[$i][id]][3] -= $leave_taken[$j][taken];     
	     }
	}
}      

return $tabledata;
}

function leaveTypes($user){

$db = &atkGetDb();
 if($user['gender'] !='Male' || $user['marital_status'] == 'Un married'){
	$sql = "SELECT id,name,no FROM leave_types where name not in ('Paternity Leave')";
	$rows = $db->getrows($sql);
    return $rows;
 } else {
   $sql = "SELECT id,name,no FROM leave_types";
   $rows = $db->getrows($sql);
   return $rows;
 }
}

function leaveUsed($user_id){
$db = &atkGetDb();
$sql = "SELECT type,name,sum(nod) as taken FROM `leave` l, `leave_types` lt where l.type=lt.id and l.created_by= '".$user_id."' and l.status='approved' and old <>1 group by lt.id";
$rows = $db->getrows($sql);  
return $rows;
}

function leaveCheck($user_id,$type_id){
$db = &atkGetDb();
$sql = "SELECT type,(no - sum(nod)) as bal FROM `leave` l, `leave_types` lt where l.type=lt.id and created_by= '".$user_id."' and type=$type_id and old <>1 group by type";
$rows = $db->getrows($sql);  
return $rows;
}

function leaveHistory($user_id,$name='')
{
$tabledata[0] = array("Leave Type","From","To","days","status","reason");
foreach($this->getHistory($user_id) as $arr)
$tabledata[] = array($arr[name],$arr[from_date],$arr[to_date],$arr[nod],$arr[status],$arr[reason]);     

$tablerenderer = &atknew("atk.utils.atktablerenderer");
$result = $tablerenderer->render($tabledata, TBL_DATA, "recordlist");
$ui = &atknew("atk.ui.atkui");
return $ui->renderBox(array("title"=>"Leave History - ".$name,"content"=>$result));  
}

function leaveHistoryHR($user_id,$name='')
{
if(count($this->getHistory($user_id))!=0)
$tabledata[0] = array("Leave Type","From","To","days","status");
foreach($this->getHistory($user_id) as $arr)
$tabledata[] = array($arr[name],$arr[from_date],$arr[to_date],$arr[nod],$arr[status]);     

$tablerenderer = &atknew("atk.utils.atktablerenderer");
$result = $tablerenderer->render($tabledata, TBL_DATA, "recordlist");
return $result;
}

function getHistory($user_id){
$db = &atkGetDb();
$sql = "SELECT lt.name,from_date,to_date,nod,status,reason FROM `leave` l, `leave_types` lt where l.type=lt.id and l.created_by= '".$user_id."' order by from_date";
$rows = $db->getrows($sql);  
return $rows;  
}

function leaveBalCheck($idx){
 $user = getUser();
  $leave_types = $this->leaveTypes($user);
  for($i=0;$i<count($leave_types);$i++){
    if($idx==$leave_types[$i][id])
     return $leave_types[$i][no];
  }
 }

function validate(&$record, $mode)
{ 
$idx = $record['type']['id'];
$user = getUser();
if($user[level]==5 && $user['id']<>$record['created_by']['id'])
$leave = $this->leaveCheck($record['created_by']['id'],$idx);
else
$leave = $this->leaveCheck($user['id'],$idx);
$from = implode('-',$record[from_date]);
$to = implode('-',$record[to_date]);
$date = $this->date_diff($from,$to);

if($this->date_fromto($from,$to)){
triggerError($record, "duration", "error_invalid_duration");
}

if($record['duration']=='Half day' && $date[days]>1){
  triggerError($record, "duration", "error_invalid_duration");
} 

if($mode=='update' && $record['cancel'] && empty($record['cancel_reason'])){
  triggerError($record, "cancel_leave", "error_obligatoryfield");
} 

if(count($leave))
 {
   if($leave[0][type]==3)
    $lbal = $leave[0][bal]+$user[el_balance];    
   else
     $lbal = $leave[0][bal];
 }
else
 { 
       if($idx==3)
	     $lbal = $user[el_balance] + $this->leaveBalCheck($idx);    
	   else 
	     $lbal =  $this->leaveBalCheck($idx); 
 }


// exit;
if($mode=='add' && ($lbal >= 0 && $lbal < $record[nod])){
   triggerError($record, "error", "You_are_exceeding_your_".$record['type']['name']."_limit");
}   

//continue with original validation
atkNode::validate($record, $mode);
}

function preAdd(&$record){

$from = implode('-',$record[from_date]);
$to = implode('-',$record[to_date]);
$date = $this->date_diff($from,$to);

$user = getUser();

if(!empty($record[created_by][id])){
    $db = &atkGetDb();
    $sql = "SELECT supervisor FROM person WHERE id = '".$record[created_by][id]."'";
    $rows = $db->getrows($sql);
    $record[supervisor] = $rows[0][supervisor];
}
else{ 
  $record[supervisor] = $user[supervisor];  
}

if($record[duration]=='Half day'){
$record[nod] =  (float)0.5;      
}
else if($record[duration]=='Full day'){
$record[nod] = $date[days];
}

return true;
}

function preUpdate(&$record){

$attrib = &$this->getAttribute("nod");
$attrib->removeFlag(AF_HIDE);
$attrib = &$this->getAttribute("status");
$attrib->removeFlag(AF_HIDE_ADD|AF_HIDE_EDIT);

$from = implode('-',$record[from_date]);
$to = implode('-',$record[to_date]);
$date = $this->date_diff($from,$to);
$record[status]='pending'; 

if($record[duration]=='Half day'){
$record[nod] =  (float)0.5;      
}
else if($record[duration]=='Full day'){
$record[nod] = $date[days];
}
if($record[cancel]){
$record[nod]=0;
$record[status]='cancelled';
}
return true;
}

function postAdd(&$record)
{  
$user = getUser();
$person = &atkGetNode('person.person');
$supervisors = $person->selectDb("person.id='".$user[supervisor]."'");
$supervisor = $supervisors[0];

$subject = "Request for ".$record['type']['name'];
$link =  "http://www.iqss.net".dispatch_url("leave.leave_approve","approve",array("atkselector"=>$record['id']));
$body =  "Dear ".$supervisor['firstname'].' '.$supervisor['lastname'].",<br><br>";
$body .= "Below are the details of employee leave application generated from IQSS e-Leave Management System.<br><br>";
$body .= "<br><br>Employee Name: ".$user[firstname].' '.$user[lastname];
$body .= "<br>Leave Type: ".$record['type']['name'];  
$body .= "<br>Leave Date: From ".implode('-',$record['from_date'])." To ".implode('-',$record['to_date']);
$body .= "<br>Reason: ".$record['reason'];
$body .= "<br><br>Your immediate action is required";
$body .= "<br><br>To Approve/Reject, click: <a href='".$link."'>".$link."</a>";
$body .= "<br><br>Please do not reply this computer generated e-mail";
$body .= "<br><br>Thank You"; 

$headers  = 'MIME-Version: 1.0' . "\r\n";
$headers .= 'Content-type: text/html; charset=iso-8859-1' . "\r\n";
$email = $user['email'];
$to_email = $supervisor['email'];
//echo dirname(__FILE__); exit;
require_once("/home/iqss/public_html/iqtimesheet/atk/ext/phpmailer/class.phpmailer.php");
//require_once("/home/iqss/public_html/iqtimesheet/atk/ext/phpmailer/language/phpmailer.lang-en.php");
$mail = new PHPMailer();
$mail->IsSMTP();
$mail->SMTPAuth = true;
$mail->SetLanguage("en", '/home/iqss/public_html/iqtimesheet/atk/ext/phpmailer/language/phpmailer.lang-en.php');
$mail->Username = "timesheet@iqss.net";
$mail->Password = "!qss!n"; 
$mail->Port = 26;
$mail->IsHTML(true);
$mail->Host = "mail.iqss.net";
$mail->IsHTML(true);
$mail->From = $user['email'];
$mail->FromName = $user[firstname].' '.$user[lastname];
$mail->AddAddress("$to_email");                  // name is optional
$mail->AddCC("sheba.kannan@iqss.net");
$mail->Subject = $subject;
$mail->Body = $body;

if(!@$mail->Send()){
   echo "Message could not be sent. <p>";
   echo "Mailer Error sub: " . $mail->ErrorInfo;
   exit;
}

return true;

}


function postUpdate(&$record)
{  
$user = getUser();

$person = &atkGetNode('person.person');
$supervisors = $person->selectDb("person.id='".$user[supervisor]."'");
$supervisor = $supervisors[0];

if($record['cancel']){

$subject = "Please cancel my ".$record['type']['name'];
$link =  "http://www.iqss.net".dispatch_url("leave.leave_approve","approve",array("atkselector"=>"leave.id=".$record['id']));
$body =  "Dear ".$supervisor['firstname'].' '.$supervisor['lastname'].",<br><br>";
$body .= "Below are the details of employee leave cancellation generated from IQSS e-Leave Management System.<br><br>";
$body .= "<br><br>Employee Name: ".$user[firstname].' '.$user[lastname];
$body .= "<br>Leave Type: ".$record['type']['name'];  
$body .= "<br>Leave Date: From ".implode('-',$record['from_date'])." To ".implode('-',$record['to_date']);
$body .= "<br>Reason: ".$record['cancel_reason'];
$body .= "<br><br>Please do not reply this computer generated e-mail";
$body .= "<br><br>Thank You"; 

$headers  = 'MIME-Version: 1.0' . "\r\n";
$headers .= 'Content-type: text/html; charset=iso-8859-1' . "\r\n";
$email = $user['email'];
$to_email = $supervisor['email'];
require_once("/home/iqss/public_html/iqtimesheet/atk/ext/phpmailer/class.phpmailer.php");
$mail = new PHPMailer();
$mail->IsSMTP();
$mail->SMTPAuth = true;
$mail->SetLanguage("en", '/home/iqss/public_html/iqtimesheet/atk/ext/phpmailer/language/phpmailer.lang-en.php');
$mail->Username = "timesheet@iqss.net";
$mail->Password = "!qss!n"; 
$mail->Port = 26;
$mail->IsHTML(true);
$mail->Host = "mail.iqss.net";
$mail->IsHTML(true);
$mail->From = $email;
$mail->FromName = $user[firstname].' '.$user[lastname];
$mail->AddAddress($to_email);                  // name is optional
$mail->AddCC("sheba.kannan@iqss.net");
$mail->Subject = $subject;
$mail->Body = $body;
if(!@$mail->Send()){
   echo "Message could not be sent. <p>";
   echo "Mailer Error: ssss" . $mail->ErrorInfo;
   exit;
}
return true;
}
return true;
}

function date_fromto($d1,$d2){
$d1 = (is_string($d1) ? strtotime($d1) : $d1);
$d2 = (is_string($d2) ? strtotime($d2) : $d2);

$diff = $d2 - $d1; 
if($diff < 0){ return true;}
}

function date_diff($d1, $d2){

$wcount = $this->weekend_counter($d1, $d2);

$d1 = (is_string($d1) ? strtotime($d1) : $d1);
$d2 = (is_string($d2) ? strtotime($d2) : $d2);

$diff_secs = abs($d1 - $d2);
$base_year = min(date("Y", $d1), date("Y", $d2));

$diff = mktime(0, 0, $diff_secs, 1, 1, $base_year);
return array(
"years" => date("Y", $diff) - $base_year,
"months_total" => (date("Y", $diff) - $base_year) * 12 + date("n", $diff) - 1,
"months" => date("n", $diff) - 1,
"days_total" => floor($diff_secs / (3600 * 24)) + 1,
"days" => floor($diff_secs / (3600 * 24)) + 1 - $wcount,
"hours_total" => floor($diff_secs / 3600),
"hours" => date("G", $diff),
"minutes_total" => floor($diff_secs / 60),
"minutes" => (int) date("i", $diff),
"seconds_total" => $diff_secs,
"seconds" => (int) date("s", $diff)
);
} 

function weekend_counter($d1,$d2){
 $sd = (is_string($d1) ? strtotime($d1) : $d1);
 $ed = (is_string($d2) ? strtotime($d2) : $d2);
 $d1 = explode('-',$d1);
 $diff = $ed- $sd; 
 $days = $diff/86400; 

 $holidays = array('15-01-2016','26-01-2016','07-03-2016','08-04-2016','06-07-2016','15-08-2016','05-09-2016','11-10-2016','31-10-2016','01-11-2016','25-12-2015');


 $count=0;
 for($d=0;$d<=$days;$d++){
   $day = date('D', mktime(0, 0, 0, date($d1[1]), date($d1[0])+$d, date($d1[2])));
     if($day == 'Sat' || $day == 'Sun'){
     $count++;
     }
   $hday = date('d-m-Y', mktime(0, 0, 0, date($d1[1]), date($d1[0])+$d, date($d1[2])));
   for($i=0;$i<count($holidays);$i++)
     if($hday == $holidays[$i]){
      $count++;
     }
 
  }   
   return $count; 
}

}
?>
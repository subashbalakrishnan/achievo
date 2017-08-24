<?php
include_once("class.leave.inc");
class leave_approve extends leave
{
  function leave_approve() 
  {
    $this->atkNode("leave_approve");
   
  }
  
  function action_admin(&$handler)
  { 
	$ui = &atknew("atk.ui.atkui");
	$theme = &atkTheme::getInstance();
	$page = &$this->getPage();
	$page->register_style($theme->stylePath("style.css"));

	$postvars = $this->m_postvars; 
	if($postvars['lsummary']==1)
	$box[] = $this->leaveBalance($postvars['uid'],$postvars['uname']); 

	if($postvars['lhistory']==1)
	$box[] = $this->leaveHistory($postvars['uid'],$postvars['uname']); 

	if($postvars['ldetails']==1)
	$box[] = $this->leaveHistory($postvars['uid'],$postvars['uname']); 

	$box[] = $ui->renderBox(array("title"=>$this->text('Leave') ." - Requests",
					     "content"=>$this->getLeaveRequests()));
	
	$box[] = $ui->renderBox(array("title"=>$this->text('Leave Cancellation') ." - Requests",
					     "content"=>$this->getLeaveCRequests()));
	
	$actionpage = $this->renderActionPage("admin", $box);
	$page->addContent($actionpage);     
  }
   
 

  function action_approve(&$handler){
   
   $leave_id = $handler->m_node->m_postvars[leave_id];
   $reason   = $handler->m_node->m_postvars[comment];
   $db = &atkGetDb();
   $user = getUser();
   $approve_disapprove = true;
  
   if($this->m_postvars["atkcancel"]=='Cancel')
   {
     $this->redirect(dispatch_url("leave.leave_approve", "admin"));
     return $handler;
   }
   
   if($this->m_postvars["atkdoapproveall"]=='Approve')
   {  $approve_disapprove = false;    
      if(is_array($this->m_postvars['leave'])){
      $list = "(".implode(',',$this->m_postvars['leave']).")";
      if($this->m_postvars["atkdocancel"]=='cancel')
      {
        $db->query("UPDATE `leave` SET `status` = 'cancelled',`cancel`=1,`nod` = 0, `approve_disapprove` = '".$reason."' WHERE `id` IN ".$list);
	$status = 'Cancelled';
      }
      else
      {
       $db->query("UPDATE `leave` SET `status` = 'approved', `approve_disapprove` = '".$reason."' WHERE `id` IN ".$list);
       $status = 'Approved';
      }
      foreach($this->m_postvars['leave'] as $id){
        $this->sendMail($id,$status,$reason);
      }
      }
   }
  
   if($this->m_postvars["atkdodisapproveall"]=='Reject')
   {  $approve_disapprove = false;
      if(is_array($this->m_postvars['leave'])){
      $list = "(".implode(',',$this->m_postvars['leave']).")";
      if($this->m_postvars["atkdocancel"]=='cancel')
      {
        $db->query("UPDATE `leave` SET `status` = 'cancelled',`cancel`=1,`nod` = 0, `approve_disapprove` = '".$reason."' WHERE `id` IN ".$list);
	$status = 'Cancelled';
      }
      else
      {  
      $db->query("UPDATE `leave` SET `status` = 'rejected',`nod` = 0, `approve_disapprove` = '".$reason."' WHERE `id` IN ".$list);
      $status = 'Rejected';
      }

      foreach($this->m_postvars['leave'] as $id){
      $this->sendMail($id,$status,$reason);
      }
     }
   }
   
   if($this->m_postvars["atkdoapprove"]=='Approve')
   { 
       $approve_disapprove = false;
       $db->query("UPDATE `leave` SET `status` = 'approved', `approve_disapprove` = '".$reason."' WHERE `id` = '".$leave_id."'");
       $this->sendMail($leave_id,'Approved',$reason);
   }
   
   if($this->m_postvars["atkdodisapprove"]=='Reject')
   {
       $approve_disapprove = false;
       $db->query("UPDATE `leave` SET `status` = 'rejected',`nod` = 0,`approve_disapprove` = '".$reason."' WHERE `id` = '".$leave_id."'");
       $this->sendMail($leave_id,'Rejected',$reason);
   }  
  
   if($approve_disapprove)
   {
    $page = &$this->getPage();
    $theme = &atkTheme::getInstance();
    $page->register_style($theme->stylePath("style.css"));
    atkimport("atk.ui.atktheme");
    $theme = &atkTheme::getInstance();
   
    $content = '<form action="'. dispatch_url("leave.leave_approve", "approve") . '" method="post">';
		$content.= session_form(SESSION_DEFAULT);
		$content.= '<table name="entryform" id="entryform" cellspacing="0">';
		$content.= '<tr><td colspan="2" class="fieldlabel"><b>' . atktext("leave_approve_disapprovegivereason", "leave") . '</b></td></tr>';
		$content.= '<tr><td colspan="2" class="fieldlabel">&nbsp;</td></tr>';
		$content.= '<tr valign="top">';
		$content.= '<td class="fieldlabel">' . atktext("reason") . ' ' . $reqimg . '</td>';
	        $content.= '<td><input id="reason" name="comment" value=""><input id="leave_id" type="hidden" name="leave_id" value="'.$handler->m_node->m_postvars[atkselector].'"></td>';
		$content.= '</tr>';
		$content.= '</table>';
		$content.= '<br>';
		$content.= '<input type="submit" class="btn_save" name="atkdoapprove" value="'.atktext('approve').'">&nbsp;&nbsp;';
		$content.= '<input type="submit" class="btn_save" name="atkdodisapprove" value="'.atktext('reject').'">&nbsp;&nbsp;';
		$content.= '<input type="submit" class="btn_cancel" name="atkcancel" value="'.atktext('cancel').'">';
		$content.= '</form>';
		$content.= '<br>';
                $title = $this->text("approve_disapprove");

		$ui = $this->getUi();
		$box = $ui->renderbox(array("title"=>$title,"content"=>$content));
		$actionpage = $this->renderActionPage("stats", $box);
		$page->addContent($actionpage);
    }
    else
    $this->redirect(dispatch_url("leave.leave_approve", "admin")); 
                return $handler;     
  }

  function action_view(&$handler){
   // print_r($handler); exit;
    $leave = &atkGetNode("leave.leave");
    $lid = $handler->m_node->m_postvars[leave_id];
   // $leaves = $leave->selectDb("id=".$lid); 
    $this->redirect(dispatch_url("leave.leave","view"), array("id"=> $leave_id)); 
    return $handler;
  }

  function getLeaveRequests()
  {
    
      // Get a reference to the hours_lock node
      
      $leaverequest = &atkGetNode("leave.leave");
      $user = getUser();
     
      // Initialize the tabledata array
      $tabledata = array();

      // Add the header columns to the tabledata
      $tabledata[] = array('Name','Type', 'From date','To date','Status','Reason',"");    
      
      if($user[level]==-1 || $user[level]==5)
      $leaves = $leaverequest->selectDb("leave.status='pending'");
      else
      $leaves = $leaverequest->selectDb("leave.supervisor='".$user['id']."' and leave.status='pending'");
      
      foreach($leaves as $leave){
        $name = $leave['created_by']['firstname'].' '.$leave['created_by']['lastname'];
	$from = implode('-',$leave["from_date"]);
	$to   = implode('-',$leave["to_date"]);
	//echo 'leave_id'.$leave['id'];
	$actions = "";
	//$actions .= "<a href='".dispatch_url("leave.leave_approve","approve",array("leave.id"=>$leave['id']))."'>Approve</a>&nbsp;&nbsp;";
	//$actions .= "<a href='".dispatch_url("leave.leave_approve","view",array("id"=>$leave['id']))."'>View</a>&nbsp;&nbsp;";
	$actions .= "<a href='".dispatch_url("leave.leave_approve","admin",array("lsummary"=>1,"uid"=>$leave['created_by']['id'],'uname'=>$name))."'>Balance</a>&nbsp;&nbsp;";
	$actions .= "<a href='".dispatch_url("leave.leave_approve","admin",array("lhistory"=>1,"uid"=>$leave['created_by']['id'],'uname'=>$name))."'>History</a>";
       
        $id = $leave['id'];
        $checkbox = "<input type='checkbox' name='leave[]' value='".$leave['id']."'>";
	$tabledata[] = array($name,$leave["type"]['name'],$from,$to,$leave['status'],$leave['reason'],$checkbox,$actions);
      }

      $tablerenderer = &atknew("atk.utils.atktablerenderer");
      $result = '<form action="'. dispatch_url("leave.leave_approve", "approve") . '" method="post">';
      if(count($leave))
      {
        $result .= 'Comment: <input id="reason" name="comment" value=""><br>';
	$result .= '<br><input type="submit" class="btn_save" name="atkdoapproveall" value="'.atktext('approve').'">&nbsp;&nbsp;';
        $result .= '<input type="submit" class="btn_save" name="atkdodisapproveall" value="'.atktext('reject').'"><br><br>';
      }
      $result .= $tablerenderer->render($tabledata, TBL_DATA, "recordlist");
     
      $result .= '</form>';
      return $result;
  }
  

  function getLeaveCRequests()
  {
    
      // Get a reference to the hours_lock node
      
      $leaverequest = &atkGetNode("leave.leave");
      $user = getUser();
     
      // Initialize the tabledata array
      $tabledata = array();

      // Add the header columns to the tabledata
      $tabledata[] = array('Name','Type', 'From date','To date','Status','Reason',"");    
      
      if($user[level]==-1 || $user[level]==5)
      $leaves = $leaverequest->selectDb("leave.status='cancel'");
      else
      $leaves = $leaverequest->selectDb("leave.supervisor='".$user['id']."' and leave.status='cancel'");
      
      foreach($leaves as $leave){
      
        $name = $leave['created_by']['firstname'].' '.$leave['created_by']['lastname'];
	$from = implode('-',$leave["from_date"]);
	$to   = implode('-',$leave["to_date"]);
	
	$actions = "";
	//$actions .= "<a href='".dispatch_url("leave.leave_approve","approve",array("leave.id"=>$leave['id']))."'>Approve</a>&nbsp;&nbsp;";
	//$actions .= "<a href='".dispatch_url("leave.leave_approve","view",array("leave.id"=>$leave['id']))."'>View</a>&nbsp;&nbsp;";
	$actions .= "<a href='".dispatch_url("leave.leave_approve","admin",array("lsummary"=>1,"uid"=>$leave['created_by']['id'],'uname'=>$name))."'>Balance</a>&nbsp;&nbsp;";
	$actions .= "<a href='".dispatch_url("leave.leave_approve","admin",array("lhistory"=>1,"uid"=>$leave['created_by']['id'],'uname'=>$name))."'>History</a>";
       
        $id = $leave['id'];
        $checkbox = "<input type='checkbox' name='leave[]' value='".$leave['id']."'>";
	$tabledata[] = array($name,$leave["type"]['name'],$from,$to,$leave['status'],$leave['cancel_reason'],$checkbox,$actions);
      }
      
      $tablerenderer = &atknew("atk.utils.atktablerenderer");
      $result = '<form action="'. dispatch_url("leave.leave_approve", "approve") . '" method="post">';
      if(count($leave))
      {
        $result .= 'Comment: <input id="reason" name="comment" value=""><br>';
	$result .= '<input type="hidden" name="atkdocancel" value="cancel"><br>';
	$result .= '<br><input type="submit" class="btn_save" name="atkdoapproveall" value="'.atktext('approve').'">&nbsp;&nbsp;';
      //$result .= '<input type="submit" class="btn_save" name="atkdodisapproveall" value="'.atktext('reject').'"><br><br>';
        $result .= '<br><br>';
      }
      $result .= $tablerenderer->render($tabledata, TBL_DATA, "recordlist");
     
      $result .= '</form>';
      return $result;
  }

function getEmail($id){

  //get user email
    $db = &atkGetDb();
    $sql = "SELECT email FROM person WHERE id = '".$id."'";
    $rows = $db->getrows($sql);
    return $rows[0][email];
}

function sendMail($leave_id,$action,$reason){

    $node = &atkGetNode('leave.leave');
    $leaves  = $node->selectDb("leave.id='".$leave_id."'");
    $leave   = $leaves[0];  

    $user = getUser();
    $person = &atkGetNode('person.person');
    $supervisors = $person->selectDb("person.id='".$user[id]."'");
    $supervisor = $supervisors[0];
   
     
    $email = $this->getEmail($leave[created_by][id]);
    
    $date_applied = sprintf("%02d-%02d-%04d %02d:%02d",$leave['created_on']["day"],$leave['created_on']["month"],$leave['created_on']["year"],$leave['created_on']["hours"],$leave['created_on']["minutes"]);
    $from = sprintf("%02d-%02d-%04d",$leave['from_date']["day"],$leave['from_date']["month"],$leave['from_date']["year"]);
    $to = sprintf("%02d-%02d-%04d",$leave['to_date']["day"],$leave['to_date']["month"],$leave['to_date']["year"]);

    
    if($action == 'Cancelled'){
    $subject = "Your request for Leave cancellation has been approved";
    $body =  "<br><br>Your request for Leave cancellation has been approved";
    }
    else    {
    $subject = "Your leave application has heen ".$action;
    $body =  "<br><br>Your leave application has heen ".$action;
    }
    $body .= "<br><br>Leave Type: ".$leave['type']['name'];
    $body .= "<br>Date Applied: ".$date_applied;
    $body .= "<br>Leave Date: From ".$from." To ".$to;
    $body .= "<br>Comments: ".$reason;

    $body .= "<br><br>Please do not reply this computer generated e-mail. If you have any enquiries, kindly refer to your
              supervisor.";
    $body .= "<br><br>Thank You";
    $headers  = 'MIME-Version: 1.0' . "\r\n";
    $headers .= 'Content-type: text/html; charset=iso-8859-1' . "\r\n";

   
	
	$mail = new atkMailer();
	$mail->IsSendmail();
	$mail->IsHTML(true);
	$mail->From = $supervisor[email];
	$mail->FromName = $supervisor[firstname].' '.$supervisor[lastname];
	$mail->AddAddress("$email");                  // name is optional
        $mail->AddCC("blesson.sebastian@iqss.co.in");
	$mail->Subject = $subject;
	$mail->Body = $body;
	if(!$mail->Send()){
	echo "Message could not be sent. <p>";
	echo "Mailer Error: " . $mail->ErrorInfo;
	exit;
	}

     
}
}
?>
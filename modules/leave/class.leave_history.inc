<?php
include_once("class.leave.inc");
class leave_history extends leave
{
  function leave_history() 
  {
    $this->atkNode("leave_history");   
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

	$box[] = $ui->renderBox(array("title"=>$this->text('Leave') ." - History",
					     "content"=>$this->getLeaveRequests()));
	
	
	$actionpage = $this->renderActionPage("admin", $box);
	$page->addContent($actionpage);     
  }
 

  function getLeaveRequests()
  {
    
      // Get a reference to the hours_lock node
      
      $employee = &atkGetNode("employee.employee");

      $user = getUser();
   
      // Initialize the tabledata array
      $tabledata = array();

      // Add the header columns to the tabledata
      $tabledata[] = array('Name','Action');    
      
      if($user[level]==-1 || $user[level]==5) {
       $leaves = $employee->selectDb("person.status='active'");
	  } 
      else {
       $leaves = $employee->selectDb("person.status='active' and person.supervisor='".$user['id']."'");
	  }
    
	foreach($leaves as $leave){
    $name = $leave['firstname'].' '.$leave['lastname'];
	$from = @implode('-', $leave["from_date"]);
	$to   = @implode('-', $leave["to_date"]);
	$actions = "";
	$actions .= "<a href='".dispatch_url("leave.leave_history","admin",array("lsummary"=>1,"uid"=>$leave['id'],'uname'=>$name))."'>Leave Balance</a>&nbsp;&nbsp;";
	$actions .= "<a href='".dispatch_url("leave.leave_history","admin",array("lhistory"=>1,"uid"=>$leave['id'],'uname'=>$name))."'>Leave History</a>";
       
  	$tabledata[] = array($name,$actions);
    }

      $tablerenderer = &atknew("atk.utils.atktablerenderer");
      $result .= $tablerenderer->render($tabledata, TBL_DATA, "recordlist");
      return $result;
  }  

}
?>
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
	
        $box[] = $ui->renderBox(array("title"=>$this->text('Employee Leave Balance'),
			             	     "content"=>$this->getLeaveBalance()));
	$actionpage = $this->renderActionPage("admin", $box);
	$page->addContent($actionpage);     
  } 
  
  function getLeaveBalance()
  {
      // Get a reference to the hours_lock node
      $employee = &atkGetNode("employee.employee");
      $user = getUser();
     
      // Initialize the tabledata array
      $tabledata = array();

      // Add the header columns to the tabledata
      $tabledata[] = array('Name',"Leave Balance");    
         
      $employees = $employee->selectDb("person.status='active'");
      $r = 1;
      $leaveCalc = array_values($this->leaveCal($uid));
      for($i=0;$i<count($leaveCalc);$i++){
	$lt .= "<th align='left'><table><tr><td colspan=3 align=center>".$leaveCalc[$i][0]."</td></tr><tr><td width=50>Allowed</td><td width=50>Taken</td><td width=50>Balance</td></tr></table></th>";
      } 
       $table = "<table class='recordlist'><thead><tr><th align='left'>Employee</th>".$lt."</tr></thead>";
       foreach($employees as $id => $employee){
               
        $uid  = $employee['id'];
	$name = $employee['firstname'].' '.$employee['lastname'];
	//$history = $this->leaveHistoryHR($uid);
	$balance = $this->leaveBalanceHR($uid);
	
	$actions = "";
	$actions .= "<a href='".dispatch_url("leave.leave_history","admin",array("lsummary"=>1,"uid"=>$uid,'uname'=>$name))."'>Balance</a>&nbsp;&nbsp;";
	$actions .= "<a href='".dispatch_url("leave.leave_history","admin",array("lhistory"=>1,"uid"=>$uid,'uname'=>$name))."'>History</a>";
             
        $id = $employee['id'];
        
	$leaveCalc = array_values($this->leaveCal($uid));
	$lt = '';
	for($i=0;$i<count($leaveCalc);$i++){
	$lt .= "<td valign='top'><table width='100%'>
	<tr><td width=50>".$leaveCalc[$i][1]."</td><td width=50>".$leaveCalc[$i][2]."</td><td width=50>".$leaveCalc[$i][3]."</td></tr>
	</table></td>";
	 }
        
	$tabledata[] = array($name,$balance);
	$table .=  "<tr ".$class."><td valign='top'>".$name."</td>".$lt."</tr>";
      }
           
      $table .= "</table>";        
      return $table;
  }
}
?>
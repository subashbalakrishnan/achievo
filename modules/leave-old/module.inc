<?php
  require_once("achievotools.inc");

  class mod_leave extends atkModule
  {
    function getMenuItems()
    {
      $this->menuitem("leaves",dispatch_url("leave.leave", "admin"),"main", array("leave.leave", "admin"));
      $this->menuitem("leave", dispatch_url("leave.leave", "admin"),"leaves", array("leave.leave", "admin"));
      $this->menuitem("leave_types", dispatch_url("leave.leave_type","admin"),"leaves", array("leave.leave_type","admin"));   
      $this->menuitem("leave_approve", dispatch_url("leave.leave_approve","admin"),"leaves", array("leave.leave_approve","admin"));    
      $this->menuitem("leave_history", dispatch_url("leave.leave_history","admin"),"leaves", array("leave.leave_history","admin"));    
    }
 
    function getNodes()
    {
       registerNode("leave.leave", array("admin", "add","view", "edit", "delete"));
       registerNode("leave.leave_type", array("admin", "add","edit", "delete"));
       registerNode("leave.leave_approve", array("admin", "add", "delete","approve","disapprove","any_user"));
       registerNode("leave.leave_history", array("admin", "add", "delete","approve","disapprove","any_user"));
    }
  }
?>
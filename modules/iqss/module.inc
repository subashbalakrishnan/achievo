<?php

  class mod_iqss extends atkModule
  {
     function getNodes()
    {
     // registerNode("reports.weekreport",array("report","view_all","view_managed"));
      //registerNode("reports.hoursurvey",array("report","view_all","view_managed"));
      registerNode("iqss.holiday",array("view"));
      registerNode("iqss.documents",array("view"));
    }

    function getMenuItems()
    {
    	
    $this->menuitem("IQSS","","main",true,800);
   
    $this->menuitem("Holidays", dispatch_url("iqss.holiday","view"), "IQSS");
    $this->menuitem("Documents", dispatch_url("iqss.documents","view"), "IQSS");
      
    $this->menuitem("-", "", "IQSS");
     
    }
  }

?>

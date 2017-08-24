<?php
class documents extends atkNode
{
  function documents() 
  {
   // $this->atkNode("Documents",NF_NO_SECURITY);
    $this->atkNode("documents");
  }
  
  // Page functions
  function action_view(&$handler)
  {
    $ui = &$this->getUi();
    if (is_object($ui))
    {
      $theme = &atkTheme::getInstance();
      $page = &$this->getPage();
      $page->register_style($theme->stylePath("style.css"));
      $page->head(atktext("cicence","setup"));
      $page->body();

     
      $box = $ui->renderBox(array("title"=>"Documents - List",
                                             "content"=>$this->getContent()));
      $actionpage = $this->renderActionPage("view", array($box));
      $page->addContent($actionpage);
    }
    else
    {
       atkerror("ui object failure");
    }
  }
  
  function getContent()
  {
    $content="1. <a href='doc/IQSSEmployeeHandbook.pdf'>IQSS Employee Handbook</a>";
    $content.="<br><br>2. <a href='doc/LeavePolicy.pdf'>Leave Policy</a>";
    $content.="<br><br>3. <a href='doc/HRManualFinal.pdf'>HR Manual</a>";
    $content.="<br><br>4. <a href='doc/RolesAndResponsibilities.pdf'>Roles and Responsibilities</a>";
    $content.="<br><br>5. <a href='doc/EmployeeReferralScheme.pdf'>Employee Referral Scheme</a>";
    $content.="<br><br>6. <a href='doc/RecruitmentProcess.pdf'>Recruitment Process</a>";
    $content.="<br><br>7. <a href='doc/PerformanceManagementSystem.pdf'>Performance Management System</a>";
    $content.="<br><br>8. <a href='doc/ExitProcess.pdf'>IQSS Exit Process</a>";
	$content.="<br><br>9. <a href='doc/SampleTemplateIQSS.pptx'>Sample Template IQSS</a>";
	$content.="<br><br>10. <a href='doc/employee_info.html'>Employee Information</a>";
	$content.="<br><br>11. <a href='doc/MedicalInsurance.pdf'>IQSS Group Medical Insurance & Personal Accident Coverage</a>";	
    return $content;    
  }
  
}

?>
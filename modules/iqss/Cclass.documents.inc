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
    $content="1. <a href='doc/IQSS-Leave Policy.pdf'>Leave Policy</a>";
    $content.="<br><br>2. <a href='doc/EMPLOYEE SERVICE RULES.pdf'>Employee Service Rules</a>";
	$content.="<br><br>3. <a href='doc/Employee of the Quarter - EOQ.pdf'>Employee of the Quarter - EOQ</a>";
	$content.="<br><br>4. <a href='doc/EOQ Nomination Form.pdf'>EOQ Nomination Form</a>";
    return $content;    
  }
  
}

?>
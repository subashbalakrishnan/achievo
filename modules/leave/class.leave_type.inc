<?php
class leave_type extends atkNode{
 
 function leave_type(){

   $this->atkNode("leave_type");
   $this->add(new atkAttribute("id", AF_AUTOKEY));
   $this->add(new atkAttribute("name", AF_OBLIGATORY|AF_UNIQUE|AF_SEARCHABLE));
   $this->add(new atkAttribute("no", AF_OBLIGATORY));
   $this->setTable("leave_types");
   $this->setOrder("name");
      
 }

 function descriptor_def(){
    return "[name]";
  }
}
?>
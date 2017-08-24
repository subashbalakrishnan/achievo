<?php /* Smarty version 2.6.11, created on 2013-04-01 04:20:22
         compiled from ./themes/default/templates/hoursurvey_footer.tpl */ ?>
<?php if ($this->_tpl_vars['saved_criteria']['toggle_save_criteria']): ?>
<div class="save_criteria">
<?php echo $this->_tpl_vars['saved_criteria']['toggle_save_criteria']; ?>
 <?php echo $this->_tpl_vars['saved_criteria']['label_save_criteria']; ?>
 <?php echo $this->_tpl_vars['saved_criteria']['save_criteria']; ?>

</div>
<div>
</div>
<?php if ($this->_tpl_vars['saved_criteria']['all_users']): ?>
<div>
<?php echo $this->_tpl_vars['saved_criteria']['all_users']; ?>
 <?php echo $this->_tpl_vars['saved_criteria']['label_all_users']; ?>
  
</div>
<?php endif; ?>
<?php endif; ?>
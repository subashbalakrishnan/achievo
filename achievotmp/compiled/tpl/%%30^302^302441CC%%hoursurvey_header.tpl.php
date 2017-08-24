<?php /* Smarty version 2.6.11, created on 2013-04-01 04:20:22
         compiled from ./themes/default/templates/hoursurvey_header.tpl */ ?>
<?php require_once(SMARTY_CORE_DIR . 'core.load_plugins.php');
smarty_core_load_plugins(array('plugins' => array(array('function', 'atkthemeicon', './themes/default/templates/hoursurvey_header.tpl', 6, false),)), $this); ?>
<?php if ($this->_tpl_vars['saved_criteria']['load_criteria']): ?>
<div class="load_criteria">
<?php echo $this->_tpl_vars['saved_criteria']['label_load_criteria']; ?>
: 
<?php echo $this->_tpl_vars['saved_criteria']['load_criteria']; ?>

<?php if ($this->_tpl_vars['saved_criteria']['forget_criteria']): ?>
  <a href="<?php echo $this->_tpl_vars['saved_criteria']['forget_criteria']; ?>
" title="<?php echo $this->_tpl_vars['saved_criteria']['label_forget_criteria']; ?>
"><img class="recordlist" border="0" title="Verwijder" alt="Verwijder" src="<?php echo smarty_function_atkthemeicon(array('name' => 'delete','type' => 'recordlist'), $this);?>
" /></a>
<?php endif; ?>
</div>
<?php endif; ?>
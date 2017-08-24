<?php /* Smarty version 2.6.11, created on 2013-04-01 04:20:14
         compiled from ./modules/timereg/themes/achievo_modern/templates/hours_adminheader.tpl */ ?>
<?php require_once(SMARTY_CORE_DIR . 'core.load_plugins.php');
smarty_core_load_plugins(array('plugins' => array(array('function', 'atkdispatchfile', './modules/timereg/themes/achievo_modern/templates/hours_adminheader.tpl', 1, false),array('function', 'atktext', './modules/timereg/themes/achievo_modern/templates/hours_adminheader.tpl', 6, false),)), $this); ?>
<form name="dayview" method="post" action="<?php echo smarty_function_atkdispatchfile(array(), $this);?>
" style="position: relative;">
<?php echo $this->_tpl_vars['sessionform']; ?>

<table border=0 cellpadding=0 cellspacing=0 width="100%" class="table">
  <tr>
    <td valign="top" align="left">
      <a href="<?php echo $this->_tpl_vars['yesterdayurl']; ?>
"><?php echo smarty_function_atktext(array('0' => 'previousday'), $this);?>
</a>&nbsp;
      <?php if ($this->_tpl_vars['todayurl'] && $this->_tpl_vars['tomorrowurl']): ?>
        <a href="<?php echo $this->_tpl_vars['todayurl']; ?>
"><?php echo smarty_function_atktext(array('id' => 'today','node' => 'houradmin'), $this);?>
</a>&nbsp;
        <a href="<?php echo $this->_tpl_vars['tomorrowurl']; ?>
"><?php echo smarty_function_atktext(array('0' => 'nextday'), $this);?>
</a>
      <?php endif; ?>
      <a href="<?php echo $this->_tpl_vars['weekviewurl']; ?>
"><?php echo smarty_function_atktext(array('0' => 'gotoweekview'), $this);?>
</a>
    </td>
    <td valign="top" align="right">
      <?php echo $this->_tpl_vars['userselect']; ?>
&nbsp;<?php echo $this->_tpl_vars['datejumper']; ?>
&nbsp;<input type="submit" value="<?php echo smarty_function_atktext(array('0' => 'goto'), $this);?>
">
    </td>
</table>
<div class="currentDate"><?php echo $this->_tpl_vars['currentdate']; ?>
 <?php echo $this->_tpl_vars['lockicon']; ?>
</div>
</form>
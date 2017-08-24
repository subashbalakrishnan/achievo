<?php /* Smarty version 2.6.11, created on 2013-03-28 06:40:50
         compiled from ./themes/default/templates/hours_adminheader_weekview.tpl */ ?>
<?php require_once(SMARTY_CORE_DIR . 'core.load_plugins.php');
smarty_core_load_plugins(array('plugins' => array(array('function', 'atkdispatchfile', './themes/default/templates/hours_adminheader_weekview.tpl', 1, false),array('function', 'atktext', './themes/default/templates/hours_adminheader_weekview.tpl', 6, false),)), $this); ?>
<form name="weekview" method="post" action="<?php echo smarty_function_atkdispatchfile(array(), $this);?>
">
  <?php echo $this->_tpl_vars['session_form']; ?>

  <table border=0 cellpadding=0 cellspacing=0 width="100%" class="table">
    <tr>
      <td valign="top" align="left">
        <a href="<?php echo $this->_tpl_vars['prevweekurl']; ?>
"><?php echo smarty_function_atktext(array('0' => 'previousweek'), $this);?>
</a>
        &nbsp;&nbsp;<a href="<?php echo $this->_tpl_vars['thisweekurl']; ?>
"><?php echo smarty_function_atktext(array('0' => 'thisweek'), $this);?>
</a>
        <?php if ($this->_tpl_vars['nextweekurl']): ?>&nbsp;&nbsp;<a href="<?php echo $this->_tpl_vars['nextweekurl']; ?>
"><?php echo smarty_function_atktext(array('0' => 'nextweek'), $this);?>
</a><?php endif; ?>
        &nbsp;&nbsp;<a href="<?php echo $this->_tpl_vars['dayviewurl']; ?>
"><?php echo smarty_function_atktext(array('0' => 'dayview'), $this);?>
</a>

        <?php if ($this->_tpl_vars['lockurl']): ?>         &nbsp;&nbsp;<a href="<?php echo $this->_tpl_vars['lockurl']; ?>
"><b><?php echo smarty_function_atktext(array('0' => 'lock'), $this);?>
</b></a>
        <?php elseif ($this->_tpl_vars['unlockurl']): ?>  &nbsp;&nbsp;<a href="<?php echo $this->_tpl_vars['unlockurl']; ?>
"><b><?php echo smarty_function_atktext(array('0' => 'unlock'), $this);?>
</b></a>
        <?php endif; ?>

      </td>
      <td valign="top" align="right">
        &nbsp;&nbsp;<?php echo $this->_tpl_vars['userselect']; ?>

        &nbsp;&nbsp;<?php echo $this->_tpl_vars['datejumper']; ?>

        &nbsp;&nbsp;<input type="submit" value="<?php echo smarty_function_atktext(array('0' => 'goto'), $this);?>
">
      </td>
    </tr>
  </table>
</form>

<b><?php echo smarty_function_atktext(array('0' => 'week'), $this);?>
 <?php echo $this->_tpl_vars['curdate']; ?>
</b>
<?php echo $this->_tpl_vars['lockicon']; ?>
 <?php if ($this->_tpl_vars['locktext']): ?>(<?php echo $this->_tpl_vars['locktext']; ?>
)<?php endif; ?>
<br><br><br>
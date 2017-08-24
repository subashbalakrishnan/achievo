Company Legal Head Alerts QUERY
------------------------------- 
  SELECT * FROM (SELECT lha.id,lha.object_id,lha.field_name, (SELECT NAME FROM compliance_nature WHERE id=C.compliance_nature_id) AS compliance_nature, c.law_category_id, 
  c.compliance_id, lha.old_value, lha.new_value, lha.ustatus,DATE_FORMAT(lha.last_Updated,'%d-%b-%Y %T'), lha.user_id,lha.what_has_changed FROM 
  (SELECT al.*, a.ustatus, a.user_id  FROM alerts al LEFT OUTER JOIN 
  (SELECT u.id, ua.user_id, ua.alerts_id, ua.status AS ustatus FROM USER u LEFT OUTER JOIN user_alerts ua ON ua.user_id = u.id WHERE u.id = 3) a 
  ON al.id=a.alerts_id) lha LEFT OUTER JOIN compliance c ON lha.object_id = c.id) clha WHERE clha.ustatus IS NULL OR clha.ustatus = 'VIEW'
  
 Escalation POINT Alerts QUERY
 ----------------------------- 

   SELECT * FROM ( SELECT lha.id,lha.object_id,lha.field_name, (SELECT NAME FROM compliance_nature WHERE id=C.compliance_nature_id) AS compliance_nature, c.law_category_id, 
  c.compliance_id, lha.old_value, lha.new_value, lha.ustatus,DATE_FORMAT(lha.last_Updated,'%d-%b-%Y %T'), lha.user_id,lha.what_has_changed FROM 
  (SELECT al.*, a.ustatus, a.user_id  FROM alerts al LEFT OUTER JOIN 
  (SELECT u.id, ua.user_id, ua.alerts_id, ua.status AS ustatus FROM USER u LEFT OUTER JOIN user_alerts ua ON ua.user_id = u.id WHERE u.id = 3) a 
  ON al.id=a.alerts_id) lha LEFT OUTER JOIN compliance c ON lha.object_id = c.id) d,  
  (SELECT DISTINCT(cm.compliance_id) AS compliance_id FROM alerts a, compliance_map cm, compliance_escalation ce WHERE
  a.object_id = cm.compliance_id AND cm.id=ce.compliance_map_id AND ce.escalation_point_id = 3 AND cm.status IS NULL AND a.date_created > cm.date_created
  UNION SELECT DISTINCT(cm.compliance_id) AS compliance_id FROM alerts a, compliance_map cm WHERE
  a.object_id = cm.compliance_id AND cm.owner_id = 3 AND cm.status IS NULL AND a.date_created > cm.date_created  
  UNION SELECT DISTINCT(cm.compliance_id) AS compliance_id FROM alerts a, compliance_map cm WHERE
  a.object_id = cm.compliance_id AND cm.approver_id = 3 AND cm.status IS NULL AND a.date_created > cm.date_created
  ) e WHERE d.object_id=e.compliance_id AND (d.ustatus IS NULL OR d.ustatus = 'VIEW')
 
<!DOCTYPE html>
<html>
<head>
<meta charset="gb2312">
<meta http-equiv="X-UA-Compatible" content="IE=edge">
<title>iEdon URP�������</title>
<meta name="viewport" content="initial-scale=1, maximum-scale=1">
<link rel="shortcut icon" href="/favicon.ico">
<meta name="apple-mobile-web-app-capable" content="yes">
<meta name="apple-mobile-web-app-status-bar-style" content="black">
<link rel="stylesheet" href="css/sm.min.css">
<link rel="stylesheet" href="css/sm-extend.min.css">
<link rel="stylesheet" href="css/project.css">
</head>
<body>
<div class="page-group">
	<div class="page">
		<header class="bar bar-nav">
		<button class="button button-link button-nav pull-left"><span class="icon icon-share"></span>&nbsp;&nbsp;<a onclick="return show_about();">����</a></button>
		<button class="button button-link button-nav pull-right"><a href="Comments.html">�²�</a>&nbsp;&nbsp;<span class="icon icon-message"></span></button>
		<h1 class="title">N������ ����������</h1>
		</header>
		<nav class="bar bar-tab">
		<a class="tab-item external active" href="index.cgi">
		<span class="icon icon-star"></span>
		<span class="tab-label">�ɼ���ѯ</span>
		</a>
		</nav>
		<div class="content">
			<div class="content-block" style="overflow: hidden">
				<canvas id="i_canvas" style="position:absolute;top:0;left:0;right:0;bottom:0"></canvas>
				 <div class="card" style="opacity: 0.85;z-index:9999">
					<div class="card-header">iEdon URP�������</div>
					<div class="card-content">
					  <div class="list-block media-list">
						<ul>
						  <li class="item-content">
							<div class="item-media">
							  <img id="main_photo" src="%s" width="70" height="84" />
							</div>
							<div class="item-inner">
							  <div class="item-subtitle"><small>������Ϣ:</small></div>
							  <div class="item-subtitle">%s</div>
							  <div class="item-subtitle">%s</div>
							</div>
						  </li>
						  <li class="item-content">
						  ��ѡ���ѯ���
						  </li>
						</ul>
						<div id="main_board">
							<div class="col-100">
								<a style="z-index:9999;margin-top:10px" title="��ѯ��ѧ�� �ɼ�+����+GPA" onclick="show_loading();" class="button button-big button-success button-fill external" href="query.cgi">��ѯ��ѧ�� �ɼ�+����+GPA</a>
							</div>
							<div class="col-100">
								<a style="z-index:9999;margin-top:10px" title="��ѧ�� �ѹ���Ŀ+����ѧ��" onclick="show_loading();" class="button button-big button-fill external" href="query.cgi?order=passed">��ѧ�� �ѹ���Ŀ+����ѧ��</a>
							</div>
							<div class="col-100">
								<a style="z-index:9999;margin-top:10px" title="��רҵ������ѯ" onclick="show_loading();" class="button button-big button-dark button-fill external" href="query.cgi?order=byplan">��רҵ������ѯ</a>
							</div>
							<div class="col-100">
								<a style="z-index:9999;margin-top:10px" title="������ɼ���ѯ" onclick="show_loading();" class="button button-big button-danger button-fill external" href="query.cgi?order=failed">������ɼ���ѯ</a>
							</div>
						</div>
					  </div>
					</div>
				   <div class="card-footer">
					  <a href="index.cgi?act=logout" class="link external">�˳���¼</a>
					</div>
		</div>
		<input id="about" style="display:none" value="<b>%s</b><br /><small>&copy; 2012-2017 iEdon Inside</br>������: %s %s<br />����˻���: %s<br />QQ: 11595137<br />N������ �� �ṩ</small>"/>
	</div>
</div>
<script type='text/javascript' src='js/zepto.min.js' charset='utf-8'></script>
<script type='text/javascript' src='js/sm.min.js' charset='utf-8'></script>
<script type='text/javascript' src='js/sm-extend.min.js' charset='utf-8'></script>
<script type='text/javascript' src='js/URPScoreHelper.js' charset='gb2312'></script>
<script>
var _mtac = {};
(function() {
    var mta = document.createElement("script");
    mta.src = "http://pingjs.qq.com/h5/stats.js?v2.0.2";
    mta.setAttribute("name", "MTAH5");
    mta.setAttribute("sid", "500426156");
    var s = document.getElementsByTagName("script")[0];
    s.parentNode.insertBefore(mta, s);
})();
</script>
<script>
var _hmt = _hmt || [];
(function() {
  var hm = document.createElement("script");
  hm.src = "https://hm.baidu.com/hm.js?0d875a45c1391fc4d011b286634b5283";
  var s = document.getElementsByTagName("script")[0]; 
  s.parentNode.insertBefore(hm, s);
})();
</script>
</body>
</html>
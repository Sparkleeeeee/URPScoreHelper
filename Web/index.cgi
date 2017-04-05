<!DOCTYPE html>
<html>
<head>
<meta charset="gb2312">
<meta http-equiv="X-UA-Compatible" content="IE=edge">
<title>iEdon URP�������</title>
<meta name="viewport" content="initial-scale=1, maximum-scale=1">
<meta name="Keywords" content="iEdon,URP�������,URPScoreHelper,����ѧԺ,����ѧԺ�ɼ���ѯ,����ѧԺ��ѧ����,����ѧԺһ������,����ѧԺURP�ۺϽ���ϵͳ,����ѧԺע������">
<meta name="description" content="�� iEdon ������ iEdon URP������֣�iEdon URPScoreHelper�������������ݴ�ѧ����ѧԺ����ҳ�棬�����������ѯ�ɼ��������Լ�GPA�ȶ��ֽ�����Ϣ��">
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
				<p id="login_query">
					�ѳɹ����&nbsp;&nbsp;<big>%ld</big>&nbsp;&nbsp;�β�ѯ����
				</p>
				������Ľ���ϵͳ�˺�����ѯ�ɼ� :)
				<canvas id="i_canvas" style="position:absolute;top:0;left:0;right:0;bottom:0"></canvas>
				<form action="main.cgi" method="post">
					<div class="list-block">
						<ul style="opacity: 0.75;z-index:9999">
							<li>
							<div class="item-content">
								<div class="item-media">
									<i class="icon icon-form-name"></i>
								</div>
								<div class="item-inner">
								<span class="icon icon-me"></span>
									<div class="item-title label">
										ѧ��
									</div>
									<div class="item-input">
										<input id="i_xh" name="xh" type="text" placeholder="������ѧ��">
									</div>
								</div>
							</div>
							</li>
							<li>
							<div class="item-content">
								<div class="item-media">
									<i class="icon icon-form-password"></i>
								</div>
								<div class="item-inner">
								<span class="icon icon-edit"></span>
									<div class="item-title label">
										����
									</div>
									<div class="item-input">
										<input name="mm" id="i_mm" type="password" placeholder="Ĭ����ѧ����ͬ">
									</div>
								</div>
							</div>
							</li>
							<li>
							<div class="item-content">
								<div class="item-media">
									<i class="icon icon-form-name"></i>
								</div>
								<div class="item-inner">
								<span class="icon icon-picture"></span>
									<div class="item-title label">
										��֤��
									</div>
									<div class="item-input">
										<input name="yzm" id="i_yzm" type="text" placeholder="�����·���֤��">
									</div>
								</div>
							</div>
							</li>
							<li class="align-top">
							<div class="item-content">
								<div class="item-media">
									<span id="login_hint">��֤��ͼ��</span><a onclick="show_loading();" href="index.cgi" class="external" title="���ˢ��"><img id="login_captcha" alt="" height="40" width="120" src="%s"/></a>
								</div>
							</div>
							</li>
						</ul>
					</div>
					<div class="content-block">
						<div class="row">
							<div class="col-100">
								<input style="z-index:9999" id="i_submit" type="submit" value="��¼" class="button button-big button-fill button-success" />
							</div>
							<div class="col-100">
								<a style="z-index:9999;margin-top:10px" title="���ܲ�ѯ���" onclick="show_loading();" class="button button-big button-fill external" href="QuickQuery.cgi">���ܲ�ѯ���</a>
							</div>
						</div>
					</div>
				</div>
			</form>
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
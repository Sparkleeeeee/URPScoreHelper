<!DOCTYPE html>
<html>
  <head>
    <meta charset="gb2312">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <title>����ѧԺ����ϵͳ - �ɼ���ѯ</title>
    <meta name="viewport" content="initial-scale=1, maximum-scale=1">
    <link rel="shortcut icon" href="/favicon.ico">
    <meta name="apple-mobile-web-app-capable" content="yes">
    <meta name="apple-mobile-web-app-status-bar-style" content="black">
    <link rel="stylesheet" href="css/sm.min.css">
    <link rel="stylesheet" href="css/sm-extend.min.css">
  </head>
 <body>
<!-- page���ϵ�����������Ŷ��ƽ�е�.page������.page��Ϊ����ҳ����·�ɿ���չʾ -->
    <div class="page-group">
        <!-- ����page ,��һ��.pageĬ�ϱ�չʾ-->
        <div class="page">
            <!-- ������ -->
            <header class="bar bar-nav">
                <h1 class="title">����ѧԺ �� ѧ���ɼ���ѯ</h1>
            </header>
            <!-- ������ -->
            <nav class="bar bar-tab">
                <a class="tab-item external active" href="index.cgi">
                    <span class="icon icon-star"></span>
                    <span class="tab-label">�ɼ���ѯ</span>
                </a>
            </nav>
            <!-- ������ҳ�������� -->
				<div class="content">
				<div class="content-block" style="overflow: hidden">
				<p>����ΪС������ṩ�� %ld �β�ѯ����~</p>
				<p>����ѧ����Ϣ����ѯ�ɼ� :)</p>
				<canvas id="i_canvas" style="position:absolute;top:0;left:0;right:0;bottom:0"></canvas>
				<form action="query.cgi" method="post">
				  <div class="list-block">
					<ul style="opacity: 0.75;z-index:9999">
					  <!-- Text inputs -->
					  <li>
						<div class="item-content">
						  <div class="item-media"><i class="icon icon-form-name"></i></div>
						  <div class="item-inner">
							<div class="item-title label">ѧ��</div>
							<div class="item-input">
							  <input id="i_xh" name="xh" type="text" placeholder="���������ѧ��">
							</div>
						  </div>
						</div>
					  </li>
					  <li>
						<div class="item-content">
						  <div class="item-media"><i class="icon icon-form-password"></i></div>
						  <div class="item-inner">
							<div class="item-title label">����</div>
							<div class="item-input">
							  <input name="mm" id="i_mm" type="password" placeholder="Ĭ��������ѧ����ͬ">
							</div>
						  </div>
						</div>
					  </li>
					  <li>
						<div class="item-content">
						  <div class="item-media"><i class="icon icon-form-name"></i></div>
						  <div class="item-inner">
							<div class="item-title label">��֤��</div>
							<div class="item-input">
							  <input name="yzm" id="i_yzm" type="text" placeholder="�������·���֤��">
							</div>
						  </div>
						</div>
					  </li>
					  <li class="align-top">
						<div class="item-content">
						  <div class="item-media">��֤��ͼ��<a href="index.cgi" title="���ˢ��"><img alt="" height="40" width="120" src="%s" /></a></div>
						</div>
					  </li>
					</ul>
				  </div>
				  <div class="content-block">
					<div class="row">
					  <div class="col-100"><input style="z-index:9999" id="i_submit" type="submit" value="��ѯ" class="button button-big button-fill button-success"></div>
					</div>
				  </div>
				</div>
				</form>
				</div>
        </div>
       <!-- �����ĵ���page����ҳ������У� -->
       <!-- <div class="page">...</div> -->
    </div>
	<script type='text/javascript' src='js/zepto.min.js' charset='utf-8'></script>
    <script type='text/javascript' src='js/sm.min.js' charset='utf-8'></script>
    <script type='text/javascript' src='js/sm-extend.min.js' charset='utf-8'></script>
	<script type='text/javascript' src='js/GuanglingScoreHelper.js' charset='gb2312'></script>
  </body>
</html>
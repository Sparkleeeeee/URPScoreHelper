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
				<form action="query.cgi" method="post">
				  <div class="list-block">
					<ul>
					  <!-- Text inputs -->
					  <li>
						<div class="item-content">
						  <div class="item-media"><i class="icon icon-form-name"></i></div>
						  <div class="item-inner">
							<div class="item-title label">ѧ��</div>
							<div class="item-input">
							  <input name="xh" type="text" placeholder="���������ѧ��">
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
							  <input name="mm" type="password" placeholder="Ĭ��������ѧ����ͬ" class="">
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
							  <input name="yzm" type="text" placeholder="�������·���֤��">
							</div>
						  </div>
						</div>
					  </li>
					  <li class="align-top">
						<div class="item-content">
						  <div class="item-media">��֤��ͼ��<img height="40" width="120"  src="%s" /></div>
						</div>
					  </li>
					</ul>
				  </div>
				  <div class="content-block">
					<div class="row">
					  <div class="col-100"><input type="submit" value="��ѯ" class="button button-big button-fill button-success"></div>
					</div>
				  </div>
				</div>
				</form>
        </div>
       <!-- �����ĵ���page����ҳ������У� -->
       <!-- <div class="page">...</div> -->
    </div>
    <script type='text/javascript' src='js/zepto.min.js' charset='utf-8'></script>
    <script type='text/javascript' src='js/sm.min.js' charset='utf-8'></script>
    <script type='text/javascript' src='js/sm-extend.min.js' charset='utf-8'></script>
    <!-- Ĭ�ϱ���Ҫִ��$.init(),ʵ��ҵ����һ�㲻����HTML�ĵ���ִ�У�ͨ������ҵ��ҳ���������ִ�� -->
    <!--<script>$.init()</script>-->
  </body>
</html>
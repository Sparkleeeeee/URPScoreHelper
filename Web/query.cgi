<!DOCTYPE html>
<html>
  <head>
    <meta charset="gb2312">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <title>%s的成绩 - 广陵学院教务系统</title>
    <meta name="viewport" content="initial-scale=1, maximum-scale=1">
    <link rel="shortcut icon" href="/favicon.ico">
    <meta name="apple-mobile-web-app-capable" content="yes">
    <meta name="apple-mobile-web-app-status-bar-style" content="black">
    <link rel="stylesheet" href="css/sm.min.css">
    <link rel="stylesheet" href="css/sm-extend.min.css">
  </head>
 <body>
<!-- page集合的容器，里面放多个平行的.page，其他.page作为内联页面由路由控制展示 -->
    <div class="page-group">
        <!-- 单个page ,第一个.page默认被展示-->
        <div class="page">
            <!-- 标题栏 -->
            <header class="bar bar-nav">
				<button class="button button-link button-nav pull-left">
					<span class="icon icon-left"></span>
					<a href="index.cgi">返回</a>
				</button>
                <h1 class="title">查询成绩 · %s</h1>
            </header>
			<!-- 工具栏 -->
            <nav class="bar bar-tab">
                <a class="tab-item external active" href="index.cgi">
                    <span class="icon icon-star"></span>
                    <span class="tab-label">成绩查询</span>
                </a>
            </nav>
			<div class="content">
			  %s
			</div>
        </div>
       <!-- 其他的单个page内联页（如果有） -->
       <!-- <div class="page">...</div> -->
    </div>
    <script type='text/javascript' src='js/zepto.min.js' charset='utf-8'></script>
    <script type='text/javascript' src='js/sm.min.js' charset='utf-8'></script>
    <script type='text/javascript' src='js/sm-extend.min.js' charset='utf-8'></script>
    <!-- 默认必须要执行$.init(),实际业务里一般不会在HTML文档里执行，通常是在业务页面代码的最后执行 -->
    <!--<script>$.init()</script>-->
  </body>
</html>
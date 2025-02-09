void CCGPainterView::BoundaryFill_4Connection(CDC *pDC, CPoint startPoint, COLORREF fillCol, COLORREF boundaryCol)
{
	////Write your boundary fill algorithm here.
	//CPoint fillPoint = startPoint;
	//COLORREF currentCol = pDC->GetPixel(fillPoint);
 //   if(currentCol != boundaryCol&&currentCol != fillCol)
	//{
	//	pDC->SetPixelV(fillPoint, fillCol);
	//	fillPoint.y = startPoint.y+1;	//(x, y+1)
 //       BoundaryFill_4Connection(pDC, fillPoint, fillCol, boundaryCol);

	//	fillPoint.y = startPoint.y-1;	//(x, y-1)
 //       BoundaryFill_4Connection(pDC, fillPoint, fillCol, boundaryCol);

	//	fillPoint.x = startPoint.x-1;
	//	fillPoint.y = startPoint.y;		//(x-1, y)
 //       BoundaryFill_4Connection(pDC, fillPoint, fillCol, boundaryCol);
	//	
	//	fillPoint.x = startPoint.x+1;	//(x+1, y)
 //       BoundaryFill_4Connection(pDC, fillPoint, fillCol, boundaryCol);

	//}

	//选做：改进之前的算法：
	//// 获取绘图区域的最大宽度和高度
	//int maxX = pDC->GetDeviceCaps(HORZRES);
	//int maxY = pDC->GetDeviceCaps(VERTRES);

	//// 使用队列进行广度优先搜索，避免栈溢出的问题
	//std::queue<std::pair<int, int>> q;

	//// 将起始点加入队列
	//q.push(std::make_pair(startPoint.x, startPoint.y));

	//// 循环处理队列中的点
	//while (!q.empty())
	//{
	//	// 取出队列头部的点
	//	std::pair<int, int> fillPoint = q.front();
	//	q.pop();

	//	// 获取当前点的颜色
	//	COLORREF currentCol = pDC->GetPixel(fillPoint.first, fillPoint.second);

	//	// 如果颜色不是边界色和填充色，则进行填充
	//	if (currentCol != boundaryCol && currentCol != fillCol)
	//	{
	//		// 设置当前点的颜色为填充色
	//		pDC->SetPixelV(fillPoint.first, fillPoint.second, fillCol);

	//		// 将相邻点加入队列
	//		if (fillPoint.second + 1 < maxY)  // 检查边界条件
	//			q.push(std::make_pair(fillPoint.first, fillPoint.second + 1)); // (x, y+1)

	//		if (fillPoint.second - 1 >= 0)
	//			q.push(std::make_pair(fillPoint.first, fillPoint.second - 1)); // (x, y-1)

	//		if (fillPoint.first - 1 >= 0)
	//			q.push(std::make_pair(fillPoint.first - 1, fillPoint.second)); // (x-1, y)

	//		if (fillPoint.first + 1 < maxX)
	//			q.push(std::make_pair(fillPoint.first + 1, fillPoint.second)); // (x+1, y)
	//	}
	//}


	//扫描线算法：
	// 获取起始点的颜色
	COLORREF startCol = pDC->GetPixel(startPoint);

	// 如果起始点的颜色是边界颜色或者已经是填充颜色，直接返回
	if (startCol == boundaryCol || startCol == fillCol)
	{
		return;
	}

	// 创建一个辅助数组用于记录每个扫描线的交点
	vector<int> intersections;

	// 初始化堆栈
	stack<CPoint> pointStack;

	// 将起始点入栈
	pointStack.push(startPoint);

	// 扫描线填充
	while (!pointStack.empty())
	{
		// 弹出栈顶点
		CPoint currentPoint = pointStack.top();
		pointStack.pop();

		// 获取当前点的颜色
		COLORREF currentCol = pDC->GetPixel(currentPoint);

		// 如果当前点的颜色不是边界颜色并且不是填充颜色
		if (currentCol != boundaryCol && currentCol != fillCol)
		{
			// 填充当前点
			pDC->SetPixel(currentPoint, fillCol);

			// 向右扫描
			CPoint rightPoint(currentPoint.x + 1, currentPoint.y);
			pointStack.push(rightPoint);

			// 向左扫描
			CPoint leftPoint(currentPoint.x - 1, currentPoint.y);
			pointStack.push(leftPoint);

			// 将当前点的上下两个相邻点入栈
			if (currentPoint.y - 1 >= 0)
			{
				CPoint upPoint(currentPoint.x, currentPoint.y - 1);
				pointStack.push(upPoint);
			}

			if (currentPoint.y + 1 < pDC->GetDeviceCaps(VERTRES))
			{
				CPoint downPoint(currentPoint.x, currentPoint.y + 1);
				pointStack.push(downPoint);
			}
		}
	}

	
}

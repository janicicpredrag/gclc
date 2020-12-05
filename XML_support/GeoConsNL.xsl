<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

<xsl:template match="/">
<html>
	<head>
		<style> 
			font.name		{font: italic;}			
		</style>
	</head>

	<body>

	<xsl:for-each select="figure">
		<br/>	
		<font color="#0000ff">
		<xsl:text>Description of construction: </xsl:text>
		</font>
		<br/>	
		<br/>	
		<hr/>
		<br/>	
		<xsl:apply-templates />
		<hr/>
		<font color="#0000ff">
		<xsl:text>Transformed by GCLC-XML package (2006)</xsl:text>
		</font>
	</xsl:for-each>

	</body>
</html>
</xsl:template>

<!--**************introducing points**************-->

<xsl:template match="define">
	<font color="#DD0000">
	<xsl:text> Let us define the following fixed points: </xsl:text>
	</font>
	<br/>	
	<ul>
	<xsl:apply-templates /> 		
	</ul>
	<br/>	
</xsl:template>


<xsl:template match="fixed_point">
	<li><xsl:text>Let </xsl:text> 
	<font class="name"><xsl:value-of select="."/></font>
	<xsl:text> be a point with Cartesian coordinates (</xsl:text>	
	<xsl:value-of select="@x"/>,
	<xsl:value-of select="@y"/>).	
	<br/>	</li>
</xsl:template>


<!--**************objects**************-->

<xsl:template match="dimensions">
	<li><xsl:text>Image dimensions: width </xsl:text><font class="name"><xsl:value-of select="@width"/></font>
	<xsl:text>mm, height </xsl:text><font class="name"><xsl:value-of select="@height"/></font><xsl:text>mm.</xsl:text>
	<br/>	</li>
</xsl:template>

<xsl:template match="fixed_number">
	<li><xsl:text>Fixed number</xsl:text>
	<font class="name"><xsl:value-of select="."/></font>
	<xsl:text> with the value </xsl:text><font class="name"><xsl:value-of select="@value"/></font>. 
	<br/>	</li>
</xsl:template>

<xsl:template match="coefficient">
	<li><xsl:text>Coefficient </xsl:text>
	<font class="name"><xsl:value-of select="."/>.</font>
	<br/>	</li>
</xsl:template>

<xsl:template match="angle">
	<li><xsl:text>Angle with the name </xsl:text> 
	<font class="name"><xsl:value-of select="."/>.</font>
	<br/>	</li>
</xsl:template>

<xsl:template match="point">
	<li><xsl:text>Point with the name </xsl:text>
	<font class="name"><xsl:value-of select="."/>.</font>
	<br/>	</li>
</xsl:template>

<xsl:template match="center">
	<li><xsl:text>Center with the name </xsl:text>
	<font class="name"><xsl:value-of select="."/>.</font>
	<br/>	</li>
</xsl:template>

<xsl:template match="segment">
	<li><xsl:text>The segment with endpoints </xsl:text>
	<font class="name"><xsl:value-of select="point[1]"/></font>
	<xsl:text> and </xsl:text><font class="name"><xsl:value-of select="point[2]"/></font>. 
	<br/></li>
</xsl:template>

<xsl:template match="vector">
	<li><xsl:text>A vector determined by the points </xsl:text>
	<font class="name"><xsl:value-of select="point[1]"/></font><xsl:text> and </xsl:text><font class="name"><xsl:value-of select="point[2]"/></font>. 
	<br/>	</li>
</xsl:template>

<xsl:template match="line">
	<li><xsl:text>Line with the name </xsl:text>
	<font class="name"><xsl:value-of select="."/>.</font>
	<br/>	</li>
</xsl:template>

<xsl:template match="line_pp">
	<li><xsl:text>A line determined by the points </xsl:text>
	<font class="name"><xsl:value-of select="point[1]"/></font>, <font class="name"><xsl:value-of select="point[2]"/></font>. 
	<br/>	</li>
</xsl:template>

<xsl:template match="circle">
	<li><xsl:text>Circle with the name </xsl:text>
	<font class="name"><xsl:value-of select="."/>.</font>
	<br/>	</li>
</xsl:template>

<xsl:template match="circle_cp">
	<li><xsl:text>A circle determined by the center </xsl:text>
	<font class="name"><xsl:value-of select="center"/></font><xsl:text> and the point </xsl:text><font class="name"><xsl:value-of select="point"/></font>. 
	<br/>	</li>
</xsl:template>

<xsl:template match="arc">
	<li><xsl:text>A arc determined by the center </xsl:text>
	<font class="name"><xsl:value-of select="center"/></font>
	<xsl:text>, point </xsl:text><font class="name"><xsl:value-of select="point"/></font>
	<xsl:text>and angle </xsl:text><font class="name"><xsl:value-of select="angle"/></font>).
	<br/></li>
</xsl:template>


<!--**************constructions**************-->

<xsl:template match="construct">
	<font color="#00AAAA">
	<xsl:text>Let us construct the following objects:</xsl:text>
	</font>
	<br/>	
	<ul>
	<xsl:apply-templates /> 
	</ul>						
	<br/>	
</xsl:template>

<xsl:template match="new_point">
	<li><xsl:text>New point with the name </xsl:text>
	<font class="name"><xsl:value-of select="."/>.</font>
	<br/>	</li>
</xsl:template>


<xsl:template match="intersection">
	<li><xsl:text>A point </xsl:text>
	<font class="name"><xsl:value-of select="new_point"/></font> 
	<xsl:text> such that it is a intersection of the lines </xsl:text>
	<font class="name"><xsl:value-of select="line[1]"/></font> and <font class="name"><xsl:value-of select="line[2]"/></font>. 
	<br/>	</li>
</xsl:template>

<xsl:template match="intersection_cc">
	<li><xsl:text>A points </xsl:text>
	<font class="name"><xsl:value-of select="new_point[1]"/></font> and <font class="name"><xsl:value-of select="new_point[2]"/></font>)  
	<xsl:text> such that they are a intersection of the circles </xsl:text>
	<font class="name"><xsl:value-of select="circle[1]"/></font>, <font class="name"><xsl:value-of select="circle[2]"/></font>. 
	<br/>	</li>
</xsl:template>

<xsl:template match="intersection_cl">
	<li><xsl:text>A points </xsl:text>
	<font class="name"><xsl:value-of select="new_point[1]"/></font> and <font class="name"><xsl:value-of select="new_point[2]"/></font>  
	<xsl:text> such that they are a intersection of the circle </xsl:text>
	<font class="name"><xsl:value-of select="circle"/></font> and the line <font class="name"><xsl:value-of select="line"/></font>.
	<br/>	</li>
</xsl:template>

<xsl:template match="midpoint">
	<li><xsl:text>A points </xsl:text>
	<font class="name"><xsl:value-of select="new_point"/></font>  
	such that it is a midpoint of the segment 
	<font class="name"><xsl:value-of select="point[1]"/></font><font class="name"><xsl:value-of select="point[2]"/></font>. 
	<br/>	</li>
</xsl:template>

<xsl:template match="foot">
	<li><xsl:text>A point </xsl:text>
	<font class="name"><xsl:value-of select="new_point"/></font>  
	<xsl:text> such that it is the foot from the point </xsl:text>
	<font class="name"><xsl:value-of select="point"/></font>
	<xsl:text> on the line </xsl:text>
<font class="name"><xsl:value-of select="line"/></font>. 
	<br/>	</li>
</xsl:template>

<xsl:template match="random_point_on_line ">
	<li><xsl:text>A point </xsl:text>
	<font class="name"><xsl:value-of select="new_point"/></font>  
	<xsl:text> such that it is a random point on line determined by points </xsl:text>
	<font class="name"><xsl:value-of select="point[1]"/></font> and <font class="name"><xsl:value-of select="point[2]"/></font>. 
	<br/>	</li>
</xsl:template>

<xsl:template match="translate">
	<li><xsl:text>A point </xsl:text>
	<font class="name"><xsl:value-of select="new_point"/></font>  
	<xsl:text> such that it is the image of the point </xsl:text>
	<font class="name"><xsl:value-of select="point"/></font>
	<xsl:text> in translation by the vector </xsl:text>
	<font class="name"><xsl:value-of select="vector/point[1]"/><xsl:value-of select="vector/point[2]"/></font>. 
	<br/>	</li>
</xsl:template>

<xsl:template match="towards">
	<li><xsl:text>A point </xsl:text>
	<font class="name"><xsl:value-of select="new_point"/></font>  
	<xsl:text> such that it is the image of the point </xsl:text>
	<font class="name"><xsl:value-of select="vector/point[1]"/></font>
	<xsl:text> in the scalling with respect to the vector </xsl:text>
	<font class="name"><xsl:value-of select="vector/point[1]"/><xsl:value-of select="vector/point[2]"/></font>
	<xsl:text> and the coefficient </xsl:text>
	<font class="name"><xsl:value-of select="coefficient"/></font>. 
	<br/>	</li>
</xsl:template>

<xsl:template match="rotate ">
	<li><xsl:text>A point </xsl:text>
	<font class="name"><xsl:value-of select="new_point"/></font>  
	<xsl:text> such that it is the image of the point </xsl:text>
	<font class="name"><xsl:value-of select="point"/></font>
	<xsl:text> in rotation with respect to the point </xsl:text>
	<font class="name"><xsl:value-of select="center"/></font>	
	<xsl:text> for the angle of </xsl:text>
 	<font class="name"><xsl:value-of select="angle"/></font>
	<xsl:text> degrees.</xsl:text>	
	<br/>	</li>
</xsl:template>

<xsl:template match="half_turn ">
	<li><xsl:text>A point </xsl:text>
	<font class="name"><xsl:value-of select="new_point"/></font>  
	<xsl:text> such that it is the image of the point </xsl:text>
	<font class="name"><xsl:value-of select="point"/></font> in half-turn with respect to the point
	<font class="name"><xsl:value-of select="center"/></font>.
	<br/>	</li>
</xsl:template>

<xsl:template match="line_reflection ">
	<li><xsl:text>A point </xsl:text>
	<font class="name"><xsl:value-of select="new_point"/></font>  
	<xsl:text> such that it is the image of the point </xsl:text>
	<font class="name"><xsl:value-of select="point"/></font>
	<xsl:text> in line reflection with respect to the line </xsl:text>
	<font class="name"><xsl:value-of select="line"/></font>.	
	<br/>	</li>
</xsl:template>

<xsl:template match="inversion ">
	<li><xsl:text>A point </xsl:text>
	<font class="name"><xsl:value-of select="new_point"/></font>  
	<xsl:text> such that it is the image of the point </xsl:text>
	<font class="name"><xsl:value-of select="point"/></font>
	<xsl:text> in inversion with respect to the circle </xsl:text>
	<font class="name"><xsl:value-of select="circle"/></font>.
	<br/>	</li>
</xsl:template>

<xsl:template match="new_line">
	<li><xsl:text>A new line </xsl:text>
	<font class="name"><xsl:value-of select="."/>.</font>
	<br/>	</li>
</xsl:template>

<xsl:template match="ruler ">
	<li><xsl:text>A line </xsl:text>
	<font class="name"><xsl:value-of select="new_line"/></font>
	<xsl:text> determined by the points </xsl:text>
	<font class="name"><xsl:value-of select="point[1]"/></font>
	<xsl:text> and </xsl:text><font class="name"><xsl:value-of select="point[2]"/></font>.		
	<br/>	</li>
</xsl:template>

<xsl:template match="parallel">
	<li><xsl:text>A line </xsl:text>
	<font class="name"><xsl:value-of select="new_line"/></font>  
	<xsl:text> such that is parallel with a line </xsl:text>
	<font class="name"><xsl:value-of select="line"/></font>
      <xsl:text>and conteins a point </xsl:text>
	<font class="name"><xsl:value-of select="point"/></font>.
	<br/>	</li>
</xsl:template>

<xsl:template match="perpendicular">
	<li><xsl:text>A line </xsl:text>
	<font class="name"><xsl:value-of select="new_line"/></font>  
	<xsl:text> such that it is the perpendicular line from the point </xsl:text>
	<font class="name"><xsl:value-of select="point"/></font>
	<xsl:text> on the line </xsl:text>
	<font class="name"><xsl:value-of select="line"/></font>.
	<br/>	</li>
</xsl:template>

<xsl:template match="segment_bisector">
	<li><xsl:text>A line </xsl:text>
	<font class="name"><xsl:value-of select="new_line"/></font>  
	<xsl:text> such that it is a bisector of the segment </xsl:text>
	<font class="name"><xsl:value-of select="point[1]"/></font><font class="name"><xsl:value-of select="point[2]"/></font>.
	<br/>	</li>
</xsl:template>

<xsl:template match="angle_bisector">
	<li><xsl:text>A line </xsl:text>
	<font class="name"><xsl:value-of select="new_line"/></font>  
	<xsl:text> such that it is a bisector of the angle determined by the points </xsl:text>
	<font class="name"><xsl:value-of select="point[1]"/></font>, <font class="name"><xsl:value-of select="point[2]"/></font> and 
	 <font class="name"><xsl:value-of select="point[3]"/></font>.
	<br/>	</li>
</xsl:template>

<xsl:template match="new_circle">
	<li><xsl:text>A new circle </xsl:text>
	<font class="name"><xsl:value-of select="."/>.</font>
	<br/>	</li>
</xsl:template>


<xsl:template match="compass ">
	<li><xsl:text>A circle </xsl:text>
	<font class="name"><xsl:value-of select="new_circle"/></font>  
	<xsl:text>with the center </xsl:text>
	<font class="name"><xsl:value-of select="center"/></font>
	<xsl:text> and conteins a point </xsl:text>
	<font class="name"><xsl:value-of select="point"/></font>.
	<br/>	</li>
</xsl:template>


<!--**************drawing**************-->





<xsl:template match="draw[@dashed='yes']">			
	<font color="#0000DD">
	<xsl:text>Let us draw (using dashed style) the following objects: </xsl:text>
	</font>
	<br/>
	<ul>
	<xsl:apply-templates /> 
	</ul>
	<br/>	
</xsl:template>

<xsl:template match="draw[@dashed='no']">			
	<font color="#0000DD">
	<xsl:text>Let us draw the following objects: </xsl:text>
	</font>
	<br/>
	<ul>
	<xsl:apply-templates /> 
	</ul>						
	<br/>
</xsl:template>

<xsl:template match="draw">			
	<font color="#0000DD">
	<xsl:text>Let us draw the following objects: </xsl:text>
	</font>
	<br/>
	<ul>
	<xsl:apply-templates /> 
	</ul>						
	<br/>
</xsl:template>


<xsl:template match="area">
	<li><xsl:text>Visible area: </xsl:text>
	<xsl:text>left-bottom corner (</xsl:text><font class="name"><xsl:value-of select="@x1"/></font>,
	<font class="name"><xsl:value-of select="@y1"/></font>),
	<xsl:text>right-top corner (</xsl:text><font class="name"><xsl:value-of select="@x2"/></font>, 
	<font class="name"><xsl:value-of select="@y2"/></font>).
	<br/>	</li>
</xsl:template>



<!--**************labeling**************-->


<xsl:template match="labels">
	<font color="#DD00DD">
	<xsl:text>Let us denote the following points:</xsl:text>
	</font>
	<br/>	
	<ul>
	<xsl:apply-templates /> 						
	</ul>
	<br/>	
</xsl:template>

<xsl:template match="point_label">
	<li><xsl:text>The point </xsl:text>
	<font class="name"><xsl:value-of select="point"/></font> in
	<font class="name"><xsl:value-of select="@direction"/></font><xsl:text> degrees direction</xsl:text>
	<xsl:choose>
		<xsl:when test="@distance"> 
		<xsl:text> and at the distance </xsl:text><font class="name"><xsl:value-of select="@distance"/></font>. 
		</xsl:when>

		<xsl:otherwise>
		<xsl:text>.</xsl:text>
		</xsl:otherwise>
	</xsl:choose>
	<br/></li>
</xsl:template>

<xsl:template match="text_label">
	<li><xsl:text>The point </xsl:text>
	<font class="name"><xsl:value-of select="point"/></font><xsl:text>, by the text </xsl:text> 
	<font class="name"><xsl:value-of select="text"/></font><xsl:text>, at </xsl:text>
	<font class="name"><xsl:value-of select="@direction"/></font><xsl:text> degrees direction</xsl:text>
	<xsl:choose>
		<xsl:when test="@distance"> 
		<xsl:text> and at the distance </xsl:text><font class="name"><xsl:value-of select="@distance"/></font>. 
		</xsl:when>

		<xsl:otherwise>
		<xsl:text>.</xsl:text>
		</xsl:otherwise>
	</xsl:choose>
	<br/></li>
</xsl:template>

<!--**************conjectures***********-->

<xsl:template match="conjecture">
	<font color="#046000">
	<xsl:text>Let us consider the following conjectures:</xsl:text><br/>
	</font>
	<ul>
	<xsl:apply-templates /> 						
	</ul>
		
	<br/>
</xsl:template>

<xsl:template match="prove">
	<li>
	<xsl:apply-templates  select="equality/expression[1]"/> = 
	<xsl:apply-templates  select="equality/expression[2]"/>
	<xsl:text> (with proof level set to </xsl:text><xsl:value-of select="@proof_level"/>
	<xsl:text> and proof limit set to </xsl:text><xsl:value-of select="@proof_limit"/>)<br/>	</li>		
</xsl:template>

<xsl:template match="expression">
	<xsl:apply-templates/>
</xsl:template>

<xsl:template match="number">		
	<xsl:value-of select="."/>		
</xsl:template>

<xsl:template match="constant">		
	<xsl:value-of select="."/>		
</xsl:template>


<xsl:template match="sum">
	(<xsl:apply-templates  select="expression[1]"/>) + 
	(<xsl:apply-templates  select="expression[2]"/>)
</xsl:template>

<xsl:template match="mult">
	(<xsl:apply-templates  select="expression[1]"/>) * 
	(<xsl:apply-templates select="expression[2]"/>)
</xsl:template>

<xsl:template match="fraction">
	(<xsl:apply-templates  select="expression[1]"/>) / 
	(<xsl:apply-templates  select="expression[2]"/>)

</xsl:template>

<xsl:template match="segment_ratio">
	segment_ratio(
	<xsl:value-of select="segment[1]/point[1]"/>,  
	<xsl:value-of select="segment[1]/point[2]"/>; 
	<xsl:value-of select="segment[2]/point[1]"/>, 
	<xsl:value-of select="segment[2]/point[2]"/>
	)
</xsl:template>

<xsl:template match="signed_area3">
	s3(
	<xsl:value-of select="point[1]"/>,  
	<xsl:value-of select="point[2]"/>, 
	<xsl:value-of select="point[3]"/>)
</xsl:template>

<xsl:template match="signed_area4">
	s4(
	<xsl:value-of select="point[1]"/>,  
	<xsl:value-of select="point[2]"/>, 
	<xsl:value-of select="point[3]"/>,
	<xsl:value-of select="point[4]"/>)
</xsl:template>

<xsl:template match="pythagoras_difference3">
	p3(
	<xsl:value-of select="point[1]"/>,  
	<xsl:value-of select="point[2]"/>, 
	<xsl:value-of select="point[3]"/>)

</xsl:template>

<xsl:template match="pythagoras_difference4">
	p4(
	<xsl:value-of select="point[1]"/>,  
	<xsl:value-of select="point[2]"/>, 
	<xsl:value-of select="point[3]"/>,
	<xsl:value-of select="point[4]"/>)
</xsl:template>




</xsl:stylesheet>
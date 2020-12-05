<?xml version="1.0" encoding="UTF-8"?>

<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

<xsl:template match="/">
<html>
	<head>
		<style> 
			p.text		{font-family : verdana; font-size : 13px}
			p.proved		{color : #0000ff; font-family : verdana; font-size : 13px}
			p.disproved		{color : #00ff00; font-family : verdana; font-size : 13px}
			p.failed		{color : #ff0000; font-family : verdana; font-size : 13px}
			td.text		{font-family : verdana; font-size : 13px}
			td.equality		{background : #ffebcd; font-family : verdana; font-size : 13px}
			td.explanation	{background : #b0e0e6; font-family : verdana; font-size : 13px}
			td.semantics	{background : #d8bfd8; font-family : verdana; font-size : 13px}
			td.notproof		{background : #f5f5dc; font-family : verdana; font-size : 13px}
			table.bordered 	{border : solid ; border-width : thin ; border-color: #cccccc}
		</style>
	</head>
	<body>
		<xsl:apply-templates/> 
	</body>
</html>
</xsl:template>

<xsl:template match="main_proof">
	<xsl:apply-templates/> 
</xsl:template>

<!-- Algebraic provers -->
  <xsl:template match="proof_title">
    <h1>
      <xsl:value-of select="."/>
    </h1>
  </xsl:template>

  <xsl:template match="proof_section">
    <h2>
      <xsl:value-of select="position()"/>&#160; <xsl:value-of select="caption"></xsl:value-of>
  </h2>
  <xsl:apply-templates/>
</xsl:template>

<xsl:template match="proof_subsection">
  <h3>
    <xsl:value-of select="1+count(parent::*/preceding-sibling::*)"></xsl:value-of>.
    <xsl:value-of select="position()-1"/> &#160;
      <xsl:value-of select="caption"></xsl:value-of>
    </h3>
    <xsl:apply-templates/>
  </xsl:template>

  <xsl:template match="caption">
  </xsl:template>

  <xsl:template match="proof_enum">
    <ul>
      <xsl:apply-templates/>
    </ul>
  </xsl:template>

  <xsl:template match="proof_enum_item">
    <li>
      <p>
      <xsl:apply-templates/>
      </p>
    </li>
  </xsl:template>

  <xsl:template match="proof_line">
    <p>
      <xsl:apply-templates/>
    </p>
  </xsl:template>

  <xsl:template match="proof_text">
    <p>
      <xsl:value-of select="."/>
      </p>
  </xsl:template>

  <xsl:template match="proof_bold">
    <p>
      <b>
        <xsl:value-of select="."/>
      </b>
    </p>
  </xsl:template>

  <xsl:template match="proof_text_bold">
      <b>
        <xsl:value-of select="."/>
      </b>
  </xsl:template>

  <xsl:template match="equality">
    <xsl:apply-templates select="expression[1]"/> =
    <xsl:apply-templates select="expression[2]"/>
  </xsl:template>

  <xsl:template match="polynomial">
    <p>
      <center>
        <xsl:value-of select="."/>
      </center>
    </p>
  </xsl:template>

  <xsl:template match="angle_tangens">
    tan(&#8737;
    <xsl:apply-templates select="point[1]"/>
    <xsl:apply-templates select="point[2]"/>
    <xsl:apply-templates select="point[3]"/>
    )
  </xsl:template>

  <xsl:template match="angle_tangens_denominator">
    (1/tan(&#8737;
    <xsl:apply-templates select="point[1]"/>
    <xsl:apply-templates select="point[2]"/>
    <xsl:apply-templates select="point[3]"/>
    ))
  </xsl:template>

  <xsl:template match="angle_tangens_numerator">
    tan(&#8737;
    <xsl:apply-templates select="point[1]"/>
    <xsl:apply-templates select="point[2]"/>
    <xsl:apply-templates select="point[3]"/>
    )
  </xsl:template>

  <xsl:template match="algebraic_sum_is_zero">
    <xsl:apply-templates select="segment[1]"/> +
    <xsl:apply-templates select="segment[2]"/> + 
    <xsl:apply-templates select="segment[3]"/> = 0
  </xsl:template>

  <xsl:template match="collinear">
    <xsl:apply-templates select="point[1]"/>,
    <xsl:apply-templates select="point[2]"/> and
    <xsl:apply-templates select="point[3]"/> are collinear.
  </xsl:template>

  <xsl:template match="parallel">
    <xsl:apply-templates select="point[1]"/>
    <xsl:apply-templates select="point[2]"/>
    &#8741;
    <xsl:apply-templates select="point[3]"/>
    <xsl:apply-templates select="point[4]"/>
  </xsl:template>

  <xsl:template match="perpendicular">
    <xsl:apply-templates select="point[1]"/>
    <xsl:apply-templates select="point[2]"/>
    &#8869;
    <xsl:apply-templates select="point[3]"/>
    <xsl:apply-templates select="point[4]"/>
  </xsl:template>

  <xsl:template match="harmonic">
    <xsl:apply-templates select="point[1]"/> and
    <xsl:apply-templates select="point[2]"/>
    are harmonic conjugate points with respect to
    <xsl:apply-templates select="point[3]"/> and
    <xsl:apply-templates select="point[4]"/>.
  </xsl:template>

  <xsl:template match="x_projection_difference">
    |
    <xsl:apply-templates select="point[1]"/>
    <xsl:apply-templates select="point[2]"/>
    |<sub>x</sub>
  </xsl:template>

  <xsl:template match="y_projection_difference">
    |
    <xsl:apply-templates select="point[1]"/>
    <xsl:apply-templates select="point[2]"/>
    |<sub>y</sub>
  </xsl:template>

  <!-- end of algebraic provers -->

<!--******** Definitions **************-->

<xsl:template match="definitions">
	<table width="98%" align="center" class="bordered">
		<tr>
			<td class="notproof">
			<b>Definitions:</b>
			</td>
		</tr>
		<xsl:for-each select="definition">
		<tr>
			<td class="notproof">
			<b><xsl:value-of select="position()"/></b>
		<xsl:value-of select="."/>
			</td>
		</tr>	
	</xsl:for-each>
	</table>	
</xsl:template>



		
	

<!--******** Proof **************-->

<xsl:template match="proof">	
	<xsl:apply-templates/> 		
</xsl:template>


<xsl:template match="proof_step">
	<table width="98%" align="center" class="bordered">
		<tr>
			<td class="text">
			<br/>
			<b>Step <xsl:value-of select="position()"/></b>
			</td>
		</tr>
		<tr>
			<td class="equality">
			<table width="100%" class="bordered">
				<tr>
					<td class="equality">
						<p class="text">
						<xsl:apply-templates select="equality/expression[1]"/> =	
						<xsl:apply-templates select="equality/expression[2]"/> 
						</p>
					</td>
				</tr>
			</table>
			</td>
		</tr>				
				
		<tr>
			<td class="explanation">
			<table width="100%" class="bordered">
				<tr>
					<td class="explanation">
					<xsl:value-of select="explanation"/>
					</td>
				</tr>

			</table>
			</td>					
		</tr>
		<tr>
			<td class="semantics">
			<table width="100%" class="bordered">			
				
				<tr>
				
					<td class="semantics">	
								
						<xsl:text>Semantic values: </xsl:text><xsl:value-of select="semantics/value[1]"/><xsl:text> = </xsl:text><xsl:value-of select="semantics/value[2]"/>
					
					</td>
				
				</tr>					
				
			</table>
			</td>					
		</tr>												
	</table>
</xsl:template>

<xsl:template match="lemma">
	<p class="text"><b>&#160;&#160;&#160;Lemma</b>
	<table width="98%" align="center" class="bordered">
	
		<tr>
			<td>
			<xsl:apply-templates select="proof"/>
			</td>					
		</tr>	
		<tr>
			<td>
			<xsl:apply-templates select="status"/>	
			</td>					
		</tr>		
	</table>
	</p>
</xsl:template>

<xsl:template match="status">
<xsl:choose>
	<xsl:when test="@value='proved'">
	<p class="proved">&#160;&#160;&#160;The conjecture has been successfuly proved.</p>
	</xsl:when>
	<xsl:when test="@value='disproved'">
	<p class="disproved">&#160;&#160;&#160;The conjecture has been disproved.</p>
	</xsl:when>
	<xsl:otherwise>
	<p class="failed">&#160;&#160;&#160;The conjecture has not been neither proved nor disproved.</p>
	</xsl:otherwise>
</xsl:choose>
	
</xsl:template>

<xsl:template match="NDGconditions">
	<table width="98%" align="center" class="bordered">
		<tr>
			<td class="notproof">
			<b>Non-degenerate conditions:</b>
			</td>
		</tr>
		<xsl:for-each select="NDGcondition">
		<tr>
			<td class="notproof">
			<b><xsl:value-of select="position()"/></b>
			<xsl:apply-templates/> 	
			</td>
		</tr>	
	</xsl:for-each>
	</table>	
</xsl:template>

<xsl:template match="inequality">
		<xsl:apply-templates select="expression[1]"/> &#8800;
		<xsl:apply-templates select="expression[2]"/> 
	
</xsl:template>

<xsl:template match="explanation">		
	<br/>&#160;&#160;
	<xsl:value-of select="."/>
</xsl:template>

<xsl:template match="prover_report">
	<table width="98%" align="center" class="bordered">
		<tr>
			<td class="notproof">
			<b>Report:</b>
			</td>
		</tr>		
		<xsl:apply-templates/>			
	</table>	
</xsl:template>


<xsl:template match="elimination_steps">
	<tr>
		<td class="notproof">
		Elimination steps = 
		<xsl:value-of select="."/>
		</td>
	</tr>		
</xsl:template>

<xsl:template match="geometrical_steps">
	<tr>
		<td class="notproof">
		Geometrical steps = 
		<xsl:value-of select="."/>
		</td>
	</tr>		
</xsl:template>

<xsl:template match="algebraic_steps">
	<tr>
		<td class="notproof">
		Algebraic steps = 
		<xsl:value-of select="."/>
		</td>
	</tr>		
</xsl:template>

<xsl:template match="total_number_of_steps">
	<tr>
		<td class="notproof">
		Total number of steps = 
		<xsl:value-of select="."/>
		</td>
	</tr>		
</xsl:template>

<xsl:template match="time">
	<tr>
		<td class="notproof">
		Time = 
		<xsl:value-of select="."/> s
		</td>
	</tr>		
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

<xsl:template match="segment">
	<xsl:apply-templates/> 
</xsl:template>

<xsl:template match="point">
		<xsl:value-of select="."/>

</xsl:template>


</xsl:stylesheet>
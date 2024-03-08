<?xml version="1.0" encoding="UTF-8"?>

<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
    <xsl:template match="/">
        <xsl:apply-templates />
    </xsl:template>

    <xsl:template match="main_proof">
        <xsl:apply-templates />
    </xsl:template>

    <xsl:template match="proof_title">
        <header>
            <xsl:value-of select="." />
        </header>
    </xsl:template>

    <xsl:template match="proof_section">
        <section>
            <header>
                <xsl:value-of select="caption"></xsl:value-of>
            </header>
            <xsl:apply-templates />
        </section>
    </xsl:template>

    <xsl:template match="proof_subsection">
        <section>
            <header>
                <xsl:value-of select="caption"></xsl:value-of>
            </header>
            <ul>
                <xsl:apply-templates />
            </ul>
        </section>
    </xsl:template>

    <xsl:template match="caption"> </xsl:template>

    <xsl:template match="proof_enum">
        <ul>
            <xsl:apply-templates />
        </ul>
    </xsl:template>

    <xsl:template match="proof_enum_item">
        <li>
            <xsl:apply-templates />
        </li>
    </xsl:template>

    <xsl:template match="proof_line">
        <xsl:apply-templates />
    </xsl:template>

    <xsl:template match="proof_text">
        <xsl:value-of select="." />
    </xsl:template>

    <xsl:template match="proof_bold">
        <p>
            <b>
                <xsl:value-of select="." />
            </b>
        </p>
    </xsl:template>

    <xsl:template match="proof_text_bold">
        <b>
            <xsl:value-of select="." />
        </b>
    </xsl:template>

    <xsl:template match="equality">
        <xsl:apply-templates select="expression[1]" /> = <xsl:apply-templates select="expression[2]" />
    </xsl:template>

    <xsl:template match="polynomial">
        <div class="math">
            <xsl:value-of select="." />
        </div>
    </xsl:template>

    <xsl:template match="angle_tangens"> \tan\left(\angle <xsl:apply-templates select="point[1]" />
    <xsl:apply-templates select="point[2]" />
    <xsl:apply-templates select="point[3]" />\right) </xsl:template>

    <xsl:template match="angle_tangens_denominator"> \frac 1{\tan\left(\angle <xsl:apply-templates select="point[1]" />
    <xsl:apply-templates select="point[2]" />
    <xsl:apply-templates select="point[3]" />
        \right)}</xsl:template>

    <xsl:template match="angle_tangens_numerator"> \tan\left(\angle <xsl:apply-templates select="point[1]" />
    <xsl:apply-templates select="point[2]" />
    <xsl:apply-templates select="point[3]" /> \right) </xsl:template>

    <xsl:template match="algebraic_sum_is_zero">
        <xsl:apply-templates select="segment[1]" /> + <xsl:apply-templates select="segment[2]" /> + <xsl:apply-templates select="segment[3]" /> = 0 </xsl:template>

    <xsl:template match="collinear"> $<xsl:apply-templates select="point[1]" />$, $<xsl:apply-templates select="point[2]" />$ and $<xsl:apply-templates select="point[3]" />$ are collinear. </xsl:template>

    <xsl:template match="parallel">
        <xsl:apply-templates select="point[1]" />
    <xsl:apply-templates select="point[2]" /> \parallel <xsl:apply-templates select="point[3]" />
    <xsl:apply-templates select="point[4]" />
    </xsl:template>

    <xsl:template match="perpendicular">
        <xsl:apply-templates select="point[1]" />
    <xsl:apply-templates select="point[2]" /> \perp <xsl:apply-templates select="point[3]" />
    <xsl:apply-templates select="point[4]" />
    </xsl:template>

    <xsl:template match="harmonic">
        <span class="inlineMath">
            <xsl:apply-templates select="point[1]" />
        </span> and <span class="inlineMath">
            <xsl:apply-templates select="point[2]" />
        </span> are harmonic conjugate points with respect to <span class="inlineMath">
            <xsl:apply-templates select="point[3]" />
        </span> and <span class="inlineMath">
            <xsl:apply-templates select="point[4]" />
        </span>. </xsl:template>

    <xsl:template match="x_projection_difference"> \lvert <xsl:apply-templates select="point[1]" />
    <xsl:apply-templates select="point[2]" /> \rvert_x </xsl:template>

    <xsl:template match="y_projection_difference"> \lvert <xsl:apply-templates select="point[1]" />
    <xsl:apply-templates select="point[2]" /> \rvert_y </xsl:template>

    <xsl:template match="definitions">
        <article id="proofView-definitions">
            <header>Definitions</header>
            <ol>
                <xsl:for-each select="definition">
                    <li>
                        <xsl:value-of select="." />
                    </li>
                </xsl:for-each>
            </ol>
        </article>
    </xsl:template>

    <xsl:template match="proof">
        <xsl:apply-templates />
    </xsl:template>

    <xsl:template match="proof_step">
        <article class='proofStep'>
            <header>Step <xsl:value-of select="position()" /></header>
            <section class='equality math'>
                <xsl:apply-templates select="equality/ expression[1]" /> = <xsl:apply-templates select="equality/ expression[2]" />
            </section>
            <section class='explanation'>
                <xsl:value-of select="explanation" />
            </section>
        </article>
    </xsl:template>

    <xsl:template match="lemma">
        <article>
            <header>Lemma</header>
            <table width="98%" align="center" class="bordered">
                <tr>
                    <td>
                        <xsl:apply-templates select="proof" />
                    </td>
                </tr>
                <tr>
                    <td>
                        <xsl:apply-templates select="status" />
                    </td>
                </tr>
            </table>
        </article>
    </xsl:template>

    <xsl:template match="status">
        <xsl:choose>
            <xsl:when test="@value='proved'">
                <p class="proved">The conjecture has been successfuly proved.</p>
            </xsl:when>
            <xsl:when test="@value='disproved'">
                <p class="disproved">The conjecture has been disproved.</p>
            </xsl:when>
            <xsl:otherwise>
                <p class="failed">The conjecture has not been neither proved nor
                    disproved.</p>
            </xsl:otherwise>
        </xsl:choose>
    </xsl:template>

    <xsl:template match="NDGconditions">
        <article id="proofView-ndgConditions">
            <header>Non-degenerate conditions</header>
            <ol>
                <xsl:for-each select="NDGcondition">
                    <li>
                        <xsl:choose>
                            <xsl:when test="equality"> $<xsl:apply-templates select="equality" />$ </xsl:when>
                            <xsl:when test="inequality"> $<xsl:apply-templates select="inequality" />$ </xsl:when>
                        </xsl:choose>
                        <xsl:value-of select="explanation">
                            <xsl:apply-templates />
                        </xsl:value-of>
                    </li>
                </xsl:for-each>
            </ol>
        </article>
    </xsl:template>

    <xsl:template match="inequality">
        <xsl:apply-templates select="expression[1]" /> \ne <xsl:apply-templates select="expression[2]" />
    </xsl:template>

    <xsl:template match="explanation">
        <xsl:value-of select="." />
    </xsl:template>

    <xsl:template match="prover_report">
        <article>
            <header>Report:</header>
            <xsl:apply-templates />
        </article>
    </xsl:template>

    <xsl:template match="elimination_steps">
        <div>Elimination steps: <xsl:value-of select="." /></div>
    </xsl:template>

    <xsl:template match="geometrical_steps">
        <div>Geometrical steps: <xsl:value-of select="." /></div>
    </xsl:template>

    <xsl:template match="algebraic_steps">
        <div>Algebraic steps: <xsl:value-of select="." /></div>
    </xsl:template>

    <xsl:template match="total_number_of_steps">
        <div>Total number of steps: <xsl:value-of select="." /></div>
    </xsl:template>

    <xsl:template match="time">
        <div>Time: <xsl:value-of select="." /> s </div>
    </xsl:template>

    <xsl:template match="expression">
        <xsl:apply-templates />
    </xsl:template>

    <xsl:template match="number">
        <xsl:value-of select="." />
    </xsl:template>

    <xsl:template match="constant">
        <xsl:value-of select="." />
    </xsl:template>

    <xsl:template match="sum"><xsl:apply-templates select="expression[1]" /> + <xsl:apply-templates select="expression[2]" /></xsl:template>

    <xsl:template match="mult"><xsl:apply-templates select="expression[1]" /> \cdot <xsl:apply-templates select="expression[2]" /></xsl:template>

    <xsl:template match="fraction"> \frac{<xsl:apply-templates select="expression[1]" />}{<xsl:apply-templates select="expression[2]" />} </xsl:template>

    <xsl:template match="segment_ratio"> \operatorname{segmentratio}\left( <xsl:value-of select="segment[1]/ point[1]" /> , <xsl:value-of select="segment[1]/ point[2]" />; <xsl:value-of
            select="segment[2]/ point[1]" />, <xsl:value-of select="segment[2]/ point[2]" /> \right) </xsl:template>

    <xsl:template match="signed_area3"> \operatorname{s}_3\left( <xsl:value-of select="point[1]" />, <xsl:value-of select="point[2]" />, <xsl:value-of select="point[3]" />\right) </xsl:template>

    <xsl:template match="signed_area4"> \operatorname{s}_4\left( <xsl:value-of select="point[1]" />, <xsl:value-of select="point[2]" />, <xsl:value-of select="point[3]" />, <xsl:value-of
            select="point[4]" />\right) </xsl:template>

    <xsl:template match="pythagoras_difference3"> \operatorname{p}_3\left( <xsl:value-of select="point[1]" />, <xsl:value-of select="point[2]" />, <xsl:value-of select="point[3]" />\right) </xsl:template>

    <xsl:template match="pythagoras_difference4"> \operatorname{p}_4\left( <xsl:value-of select="point[1]" />, <xsl:value-of select="point[2]" />, <xsl:value-of select="point[3]" />, <xsl:value-of
            select="point[4]" />\right) </xsl:template>

    <xsl:template match="segment">
        <xsl:apply-templates />
    </xsl:template>

    <xsl:template match="point">
        <xsl:value-of select="." />
    </xsl:template>
</xsl:stylesheet>
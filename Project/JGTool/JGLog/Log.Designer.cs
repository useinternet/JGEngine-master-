﻿namespace JGLog
{
    partial class Log
    {
        /// <summary>
        /// 필수 디자이너 변수입니다.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 사용 중인 모든 리소스를 정리합니다.
        /// </summary>
        /// <param name="disposing">관리되는 리소스를 삭제해야 하면 true이고, 그렇지 않으면 false입니다.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form 디자이너에서 생성한 코드

        /// <summary>
        /// 디자이너 지원에 필요한 메서드입니다. 
        /// 이 메서드의 내용을 코드 편집기로 수정하지 마세요.
        /// </summary>
        private void InitializeComponent()
        {
            this.LogBox = new System.Windows.Forms.RichTextBox();
            this.SuspendLayout();
            // 
            // LogBox
            // 
            this.LogBox.BackColor = System.Drawing.SystemColors.ScrollBar;
            this.LogBox.Dock = System.Windows.Forms.DockStyle.Fill;
            this.LogBox.ImeMode = System.Windows.Forms.ImeMode.NoControl;
            this.LogBox.Location = new System.Drawing.Point(0, 0);
            this.LogBox.Name = "LogBox";
            this.LogBox.ReadOnly = true;
            this.LogBox.RightToLeft = System.Windows.Forms.RightToLeft.No;
            this.LogBox.Size = new System.Drawing.Size(782, 553);
            this.LogBox.TabIndex = 1;
            this.LogBox.Text = "";
            // 
            // JGLog
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 15F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(782, 553);
            this.Controls.Add(this.LogBox);
            this.ImeMode = System.Windows.Forms.ImeMode.Off;
            this.Name = "JGLog";
            this.Text = "JGLog";
            this.Load += new System.EventHandler(this.JGLog_Load);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.RichTextBox LogBox;
    }
}
